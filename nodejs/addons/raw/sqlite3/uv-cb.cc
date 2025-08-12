#include <node.h>
#include <node_buffer.h>
#include <assert.h>
#include <sqlite3.h>
#include <uv.h>
#include <vector>
#include <string>

using namespace v8;

sqlite3* db;  // Global database handle

// Structure to hold query results
struct QueryResult {
    std::vector<std::string> colNames;
    std::vector<std::vector<std::string>> rows;
    std::string errorMsg;
    int rc;
};

// Structure for async work
struct AsyncQueryBaton {
    uv_work_t request;
    Persistent<Function> callback;
    std::string query;
    QueryResult result;
    Isolate* isolate;
};

// Callback function to collect query results
static int callback(void* data, int argc, char** argv, char** colName) {
    QueryResult* result = static_cast<QueryResult*>(data);
    
    if (result->colNames.empty()) {
        for (int i = 0; i < argc; i++) {
            result->colNames.push_back(colName[i] ? colName[i] : "");
        }
    }
    
    std::vector<std::string> row;
    for (int i = 0; i < argc; i++) {
        row.push_back(argv[i] ? argv[i] : "NULL");
    }
    result->rows.push_back(row);
    
    return 0;
}

void OpenDb(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    Local<Context> context = isolate->GetCurrentContext();
    HandleScope scope(isolate);

    assert(args[0]->IsString());  // Database path
    
    String::Utf8Value dbPath(isolate, args[0].As<String>());
    int rc = sqlite3_open(*dbPath, &db);
    
    args.GetReturnValue().Set(Integer::New(isolate, rc));
}

// Work function executed in the thread pool
void ExecuteQuery(uv_work_t* req) {
    AsyncQueryBaton* baton = static_cast<AsyncQueryBaton*>(req->data);
    
    char* errMsg = nullptr;
    baton->result.rc = sqlite3_exec(db, baton->query.c_str(), callback, &baton->result, &errMsg); // Fix: Use baton->result.rc
    
    if (errMsg) {
        baton->result.errorMsg = errMsg;
        sqlite3_free(errMsg);
    }
}

// Callback function executed after work is complete
void AfterExecuteQuery(uv_work_t* req, int status) {
    Isolate* isolate = static_cast<AsyncQueryBaton*>(req->data)->isolate;
    HandleScope scope(isolate);
    AsyncQueryBaton* baton = static_cast<AsyncQueryBaton*>(req->data);
    
    Local<Context> context = isolate->GetCurrentContext();
    Local<Value> argv[2];
    
    if (baton->result.rc != SQLITE_OK) { // Fix: Use baton->result.rc
        // Pass error to callback
        argv[0] = Exception::Error(
            String::NewFromUtf8(isolate, baton->result.errorMsg.c_str()).ToLocalChecked()
        );
        argv[1] = Undefined(isolate);
    } else {
        // Convert query results to JavaScript array of objects
        Local<Array> jsResult = Array::New(isolate, baton->result.rows.size());
        for (size_t i = 0; i < baton->result.rows.size(); i++) {
            Local<Object> rowObj = Object::New(isolate);
            for (size_t j = 0; j < baton->result.colNames.size(); j++) {
                Local<String> key = String::NewFromUtf8(isolate, baton->result.colNames[j].c_str()).ToLocalChecked();
                Local<String> value = String::NewFromUtf8(isolate, baton->result.rows[i][j].c_str()).ToLocalChecked();
                rowObj->Set(context, key, value).Check();
            }
            jsResult->Set(context, i, rowObj).Check();
        }
        argv[0] = Undefined(isolate);
        argv[1] = jsResult;
    }
    
    // Call the JavaScript callback
    Local<Function> cb = Local<Function>::New(isolate, baton->callback);
    cb->Call(context, Undefined(isolate), 2, argv).ToLocalChecked();
    
    // Clean up
    baton->callback.Reset();
    delete baton;
}

void Exec(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    Local<Context> context = isolate->GetCurrentContext();
    HandleScope scope(isolate);

    assert(args[0]->IsString());  // SQL query
    assert(args[1]->IsFunction()); // Callback function
    
    AsyncQueryBaton* baton = new AsyncQueryBaton();
    baton->request.data = baton;
    baton->isolate = isolate;
    
    String::Utf8Value querytxt(isolate, args[0].As<String>());
    baton->query = std::string(*querytxt);
    
    baton->callback.Reset(isolate, args[1].As<Function>());
    
    // Queue the work
    uv_queue_work(uv_default_loop(), &baton->request, ExecuteQuery, AfterExecuteQuery);
    
    args.GetReturnValue().Set(Undefined(isolate));
}

void Initialize(Local<Object> exports) {
    NODE_SET_METHOD(exports, "OpenDb", OpenDb);
    NODE_SET_METHOD(exports, "Exec", Exec);
}

NODE_MODULE(js_executor, Initialize)