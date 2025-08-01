```mermaid
classDiagram
  direction RL
  
  class Data{
    +bool IsValue () const
    +bool IsModule () const
    +bool IsModuleRequest () const
    +bool IsFixedArray () const
    +bool IsPrivate () const
    +bool IsObjectTemplate () const
    +bool IsFunctionTemplate () const
    +bool IsContext () const
  }
  class CompileHintsCollector{
    std::vector< int > 	GetCompileHints (Isolate *isolate) const
  }
  class Context{
    Local< Object > Global ()
    void DetachGlobal ()
    void SetSecurityToken (Local< Value > token)
    void UseDefaultSecurityToken ()
    Local< Value > GetSecurityToken ()
    void Enter ()
    void Exit ()
    Maybe< void > DeepFreeze (DeepFreezeDelegate *delegate=nullptr)
    Isolate * GetIsolate ()
    MicrotaskQueue * GetMicrotaskQueue ()
    void SetMicrotaskQueue (MicrotaskQueue *queue)
    uint32_t GetNumberOfEmbedderDataFields ()
    Local< Value > GetEmbedderData (int index)
    Local< Object > GetExtrasBindingObject ()
    void SetEmbedderData (int index, Local< Value > value)
    void * GetAlignedPointerFromEmbedderData (Isolate *isolate, int index)
    void * GetAlignedPointerFromEmbedderData (int index)
    void SetAlignedPointerInEmbedderData (int index, void *value)
    void AllowCodeGenerationFromStrings (bool allow)
    bool IsCodeGenerationFromStringsAllowed () const
    void SetErrorMessageForCodeGenerationFromStrings (Local< String > message)
    void SetErrorMessageForWasmCodeGeneration (Local< String > message)
    MaybeLocal< T > GetDataFromSnapshotOnce (size_t index)
    void SetAbortScriptExecution (AbortScriptExecutionCallback callback)
    void SetPromiseHooks (Local< Function > init_hook, Local< Function > before_hook, Local< Function > after_hook, Local< Function > resolve_hook)
    bool HasTemplateLiteralObject (Local< Value > object)
    MaybeLocal< T > GetDataFromSnapshotOnce (size_t index)
  }
  class FixedArray{
    int Length () const
    Local< Data > Get (Local< Context > context, int i) const
    FixedArray * Cast (Data *data)
  }
  class Module{
    Status GetStatus () const
    Local< Value > GetException () const
    Local< FixedArray > GetModuleRequests () const
    Location SourceOffsetToLocation (int offset) const
    int GetIdentityHash () const
    Maybe< bool > InstantiateModule (Local< Context > context, ResolveModuleCallback module_callback, ResolveSourceCallback source_callback=nullptr)
    MaybeLocal< Value > Evaluate (Local< Context > context)
    Local< Value > GetModuleNamespace ()
    Local< UnboundModuleScript > GetUnboundModuleScript ()
    int ScriptId () const
    bool IsGraphAsync () const
    bool HasTopLevelAwait () const
    bool IsSourceTextModule () const
    bool IsSyntheticModule () const
    Maybe< bool > SetSyntheticModuleExport (Isolate *isolate, Local< String > export_name, Local< Value > export_value)
    std::pair< LocalVector< Module >, LocalVector< Message > > GetStalledTopLevelAwaitMessages (Isolate *isolate)
    static Local< Module > CreateSyntheticModule (Isolate *isolate, Local< String > module_name, const MemorySpan< const Local< String > > &export_names, SyntheticModuleEvaluationSteps evaluation_steps)
    static V8_INLINE Module * Cast (Data *data)
  }
  class ModuleRequest{
    Local< String > GetSpecifier () const
    ModuleImportPhase GetPhase () const
    int GetSourceOffset () const
    Local< FixedArray > GetImportAttributes () const
    Local< FixedArray > GetImportAssertions () const
    static V8_INLINE ModuleRequest * Cast (Data *data)
  }
  class PrimitiveArray{
    int Length () const
    void Set (Isolate *isolate, int index, Local< Primitive > item)
    Local< Primitive > Get (Isolate *isolate, int index)
    static Local< PrimitiveArray > New (Isolate *isolate, int length)
    static V8_INLINE PrimitiveArray * Cast (Data *data)
  }
  class Private{
    Local< Value > Name () const
    static Local< Private > New (Isolate *isolate, Local< String > name=Local< String >())
    static Local< Private > ForApi (Isolate *isolate, Local< String > name)
    static V8_INLINE Private * Cast (Data *data)
  }
  class Script{
    MaybeLocal< Value > Run (Local< Context > context)
    MaybeLocal< Value > Run (Local< Context > context, Local< Data > host_defined_options)
    Local< UnboundScript > GetUnboundScript ()
    Local< Value > GetResourceName ()
    std::vector< int > GetProducedCompileHints () const
    Local< CompileHintsCollector > GetCompileHintsCollector () const
    static MaybeLocal< Script > Compile (Local< Context > context, Local< String > source, ScriptOrigin *origin=nullptr)
  }
  class Signature{
    static Local< Signature > New (Isolate *isolate, Local< FunctionTemplate > receiver=Local< FunctionTemplate >())
    static V8_INLINE Signature * Cast (Data *data)
  }
  class Template{
    void Set (Local< Name > name, Local< Data > value, PropertyAttribute attributes=None)
    void SetPrivate (Local< Private > name, Local< Data > value, PropertyAttribute attributes=None)
    void Set (Isolate *isolate, const char *name, Local< Data > value, PropertyAttribute attributes=None)
    void SetAccessorProperty (Local< Name > name, Local< FunctionTemplate > getter=Local< FunctionTemplate >(), Local< FunctionTemplate > setter=Local< FunctionTemplate >(), PropertyAttribute attribute=None)
    void SetNativeDataProperty (Local< Name > name, AccessorNameGetterCallback getter, AccessorNameSetterCallback setter=nullptr, Local< Value > data=Local< Value >(), PropertyAttribute attribute=None, SideEffectType getter_side_effect_type=SideEffectType::kHasSideEffect, SideEffectType setter_side_effect_type=SideEffectType::kHasSideEffect)
    void SetLazyDataProperty (Local< Name > name, AccessorNameGetterCallback getter, Local< Value > data=Local< Value >(), PropertyAttribute attribute=None, SideEffectType getter_side_effect_type=SideEffectType::kHasSideEffect, SideEffectType setter_side_effect_type=SideEffectType::kHasSideEffect)
    void SetIntrinsicDataProperty (Local< Name > name, Intrinsic intrinsic, PropertyAttribute attribute=None)
  }
  class UnboundModuleScript{
    Local< Value > GetSourceURL ()
    Local< Value > GetSourceMappingURL ()
  }
  class UnboundScript{
    Local< Script > BindToCurrentContext ()
    int GetId () const
    Local< Value > GetScriptName ()
    Local< Value > GetSourceURL ()
    Local< Value > GetSourceMappingURL ()
    int GetLineNumber (int code_pos=0)
    int GetColumnNumber (int code_pos=0)
    static const int kNoScriptId = 0
  }
  class Value{
    bool IsUndefined () const
    bool IsNull () const
    bool IsNullOrUndefined () const
    bool IsTrue () const
    bool IsFalse () const
    bool IsName () const
    bool IsString () const
    bool IsSymbol () const
    bool IsFunction () const
    bool IsArray () const
    bool IsObject () const
    bool IsBigInt () const
    bool IsBoolean () const
    bool IsNumber () const
    bool IsExternal () const
    bool IsInt32 () const
    bool IsUint32 () const
    bool IsDate () const
    bool IsArgumentsObject () const
    bool IsBigIntObject () const
    bool IsBooleanObject () const
    bool IsNumberObject () const
    bool IsStringObject () const
    bool IsSymbolObject () const
    bool IsNativeError () const
    bool IsRegExp () const
    bool IsAsyncFunction () const
    bool IsGeneratorFunction () const
    bool IsGeneratorObject () const
    bool IsPromise () const
    bool IsMap () const
    bool IsSet () const
    bool IsMapIterator () const
    bool IsSetIterator () const
    bool IsWeakMap () const
    bool IsWeakSet () const
    bool IsWeakRef () const
    bool IsArrayBuffer () const
    bool IsArrayBufferView () const
    bool IsTypedArray () const
    bool IsUint8Array () const
    bool IsUint8ClampedArray () const
    bool IsInt8Array () const
    bool IsUint16Array () const
    bool IsInt16Array () const
    bool IsUint32Array () const
    bool IsInt32Array () const
    bool IsFloat16Array () const
    bool IsFloat32Array () const
    bool IsFloat64Array () const
    bool IsBigInt64Array () const
    bool IsBigUint64Array () const
    bool IsDataView () const
    bool IsSharedArrayBuffer () const
    bool IsProxy () const
    bool IsWasmMemoryObject () const
    bool IsWasmMemoryMapDescriptor () const
    bool IsWasmModuleObject () const
    bool IsWasmNull () const
    bool IsModuleNamespaceObject () const
    bool IsPrimitive () const
    MaybeLocal< Primitive > ToPrimitive (Local< Context > context) const
    MaybeLocal< Numeric > ToNumeric (Local< Context > context) const
    MaybeLocal< BigInt > ToBigInt (Local< Context > context) const
    MaybeLocal< Number > ToNumber (Local< Context > context) const
    MaybeLocal< String > ToString (Local< Context > context) const
    MaybeLocal< String > ToDetailString (Local< Context > context) const
    MaybeLocal< Object > ToObject (Local< Context > context) const
    MaybeLocal< Integer > ToInteger (Local< Context > context) const
    MaybeLocal< Uint32 > ToUint32 (Local< Context > context) const
    MaybeLocal< Int32 > ToInt32 (Local< Context > context) const
    Local< Boolean > ToBoolean (Isolate *isolate) const
    MaybeLocal< Uint32 > ToArrayIndex (Local< Context > context) const
    bool BooleanValue (Isolate *isolate) const
    Maybe< double > NumberValue (Local< Context > context) const
    Maybe< int64_t > IntegerValue (Local< Context > context) const
    Maybe< uint32_t > Uint32Value (Local< Context > context) const
    Maybe< int32_t > Int32Value (Local< Context > context) const
    Maybe< bool > Equals (Local< Context > context, Local< Value > that) const
    bool StrictEquals (Local< Value > that) const
    bool SameValue (Local< Value > that) const
    Local< String > TypeOf (Isolate *)
    Maybe< bool > InstanceOf (Local< Context > context, Local< Object > object)
    uint32_t GetHash ()
  }
  class FunctionTemplate{
    MaybeLocal< Function > GetFunction (Local< Context > context)
    MaybeLocal< Object > NewRemoteInstance ()
    void SetCallHandler (FunctionCallback callback, Local< Value > data=Local< Value >(), SideEffectType side_effect_type=SideEffectType::kHasSideEffect, const MemorySpan< const CFunction > &c_function_overloads)
    void SetLength (int length)
    Local< ObjectTemplate > InstanceTemplate ()
    void Inherit (Local< FunctionTemplate > parent)
    Local< ObjectTemplate > PrototypeTemplate ()
    void SetPrototypeProviderTemplate (Local< FunctionTemplate > prototype_provider)
    void SetClassName (Local< String > name)
    void SetInterfaceName (Local< String > name)
    void SetExceptionContext (ExceptionContext context)
    void SetAcceptAnyReceiver (bool value)
    void ReadOnlyPrototype ()
    void RemovePrototype ()
    bool HasInstance (Local< Value > object)
    bool IsLeafTemplateForApiObject (v8::Local< v8::Value > value) const
    void SealAndPrepareForPromotionToReadOnly ()
    static Local< FunctionTemplate > New (Isolate *isolate, FunctionCallback callback=nullptr, Local< Value > data=Local< Value >(), Local< Signature > signature=Local< Signature >(), int length=0, ConstructorBehavior behavior=ConstructorBehavior::kAllow, SideEffectType side_effect_type=SideEffectType::kHasSideEffect, const CFunction *c_function=nullptr, uint16_t instance_type=0, uint16_t allowed_receiver_instance_type_range_start=0, uint16_t allowed_receiver_instance_type_range_end=0)
    static Local< FunctionTemplate > NewWithCFunctionOverloads (Isolate *isolate, FunctionCallback callback=nullptr, Local< Value > data=Local< Value >(), Local< Signature > signature=Local< Signature >(), int length=0, ConstructorBehavior behavior=ConstructorBehavior::kAllow, SideEffectType side_effect_type=SideEffectType::kHasSideEffect, const MemorySpan< const CFunction > &c_function_overloads)
    static Local< FunctionTemplate > NewWithCache (Isolate *isolate, FunctionCallback callback, Local< Private > cache_property, Local< Value > data=Local< Value >(), Local< Signature > signature=Local< Signature >(), int length=0, SideEffectType side_effect_type=SideEffectType::kHasSideEffect)
    static FunctionTemplate * Cast (Data *data)
  }
  class ObjectTemplate{
    MaybeLocal< Object > NewInstance (Local< Context > context)
    void SetHandler (const NamedPropertyHandlerConfiguration &configuration)
    void SetHandler (const IndexedPropertyHandlerConfiguration &configuration)
    void SetCallAsFunctionHandler (FunctionCallback callback, Local< Value > data=Local< Value >())
    void MarkAsUndetectable ()
    void SetAccessCheckCallback (AccessCheckCallback callback, Local< Value > data=Local< Value >())
    void SetAccessCheckCallbackAndHandler (AccessCheckCallback callback, const NamedPropertyHandlerConfiguration &named_handler, const IndexedPropertyHandlerConfiguration &indexed_handler, Local< Value > data=Local< Value >())
    int InternalFieldCount () const
    void SetInternalFieldCount (int value)
    bool IsImmutableProto () const
    void SetImmutableProto ()
    void SetCodeLike ()
    bool IsCodeLike () const 
    static Local< ObjectTemplate > New (Isolate *isolate, Local< FunctionTemplate > constructor=Local< FunctionTemplate >())
    static ObjectTemplate * Cast (Data *data)
  }
  class External{
    void * Value () const
    static Local< External > New (Isolate *isolate, void *value)
    static External * Cast (Value *value)
  }
  class Object{
    Maybe< bool > Set (Local< Context > context, Local< Value > key, Local< Value > value)
    Maybe< bool > Set (Local< Context > context, Local< Value > key, Local< Value > value, MaybeLocal< Object > receiver)
    Maybe< bool > Set (Local< Context > context, uint32_t index, Local< Value > value)
    Maybe< bool > CreateDataProperty (Local< Context > context, Local< Name > key, Local< Value > value)
    Maybe< bool > CreateDataProperty (Local< Context > context, uint32_t index, Local< Value > value)
    Maybe< bool > DefineOwnProperty (Local< Context > context, Local< Name > key, Local< Value > value, PropertyAttribute attributes=None)
    Maybe< bool > DefineProperty (Local< Context > context, Local< Name > key, PropertyDescriptor &descriptor)
    MaybeLocal< Value > Get (Local< Context > context, Local< Value > key)
    MaybeLocal< Value > Get (Local< Context > context, Local< Value > key, MaybeLocal< Object > receiver)
    MaybeLocal< Value > Get (Local< Context > context, uint32_t index)
    Maybe< PropertyAttribute > GetPropertyAttributes (Local< Context > context, Local< Value > key)
    MaybeLocal< Value > GetOwnPropertyDescriptor (Local< Context > context, Local< Name > key)
    Maybe< bool > Has (Local< Context > context, Local< Value > key)
    Maybe< bool > Delete (Local< Context > context, Local< Value > key)
    Maybe< bool > Has (Local< Context > context, uint32_t index)
    Maybe< bool > Delete (Local< Context > context, uint32_t index)
    void SetAccessorProperty (Local< Name > name, Local< Function > getter, Local< Function > setter=Local< Function >(), PropertyAttribute attributes=None)
    Maybe< bool > SetNativeDataProperty (Local< Context > context, Local< Name > name, AccessorNameGetterCallback getter, AccessorNameSetterCallback setter=nullptr, Local< Value > data=Local< Value >(), PropertyAttribute attributes=None, SideEffectType getter_side_effect_type=SideEffectType::kHasSideEffect, SideEffectType setter_side_effect_type=SideEffectType::kHasSideEffect)
    Maybe< bool > SetLazyDataProperty (Local< Context > context, Local< Name > name, AccessorNameGetterCallback getter, Local< Value > data=Local< Value >(), PropertyAttribute attributes=None, SideEffectType getter_side_effect_type=SideEffectType::kHasSideEffect, SideEffectType setter_side_effect_type=SideEffectType::kHasSideEffect)
    Maybe< bool > HasPrivate (Local< Context > context, Local< Private > key)
    Maybe< bool > SetPrivate (Local< Context > context, Local< Private > key, Local< Value > value)
    Maybe< bool > DeletePrivate (Local< Context > context, Local< Private > key)
    MaybeLocal< Value > GetPrivate (Local< Context > context, Local< Private > key)
    MaybeLocal< Array > GetPropertyNames (Local< Context > context)
    MaybeLocal< Array > GetPropertyNames (Local< Context > context, KeyCollectionMode mode, PropertyFilter property_filter, IndexFilter index_filter, KeyConversionMode key_conversion=KeyConversionMode::kKeepNumbers)
    MaybeLocal< Array > GetOwnPropertyNames (Local< Context > context)
    MaybeLocal< Array > GetOwnPropertyNames (Local< Context > context, PropertyFilter filter, KeyConversionMode key_conversion=KeyConversionMode::kKeepNumbers)
    Local< Value > GetPrototype()
    Local< Value > GetPrototypeV2 ()
    Maybe< bool > SetPrototype(Local< Context > context
    Maybe< bool > SetPrototypeV2 (Local< Context > context, Local< Value > prototype)
    Local< Object > FindInstanceInPrototypeChain (Local< FunctiTemplate > tmpl)
    MaybeLocal< String > ObjectProtoToString (Local< Context > context)
    Local< String > GetConstructorName ()
    Maybe< bool > SetIntegrityLevel (Local< Context > context, IntegrityLevel level)
    int InternalFieldCount () const Local< Data > GetInternalField (int index)
    void SetInternalField (int index, Local< Data > data) void * GetAlignedPointerFromInternalField (int index) void * GetAlignedPointerFromInternalField (v8::Isolate *isolate, int index)
    void SetAlignedPointerInInternalField (int index, void *value)
    void SetAlignedPointerInInternalFields (int argc, int indices[], void *values[])
    Maybe< bool > HasOwnProperty (Local< Context > context, Local< Name > key)
    Maybe< bool > HasOwnProperty (Local< Context > context, uint32_t index)
    Maybe< bool > HasRealNamedProperty (Local< Context > context, Local< Name > key)
    Maybe< bool > HasRealIndexedProperty (Local< Context > context, uint32_t index)
    Maybe< bool > HasRealNamedCallbackProperty (Local< Context > context, Local< Name > key)
    MaybeLocal< Value > GetRealNamedPropertyInPrototypeChain (Local< Context > context, Local< Name > key)
    Maybe< PropertyAttribute > GetRealNamedPropertyAttributesInPrototypeChain (Local< Context > context, Local< Name > key)
    MaybeLocal< Value > GetRealNamedProperty (Local< Context > context, Local< Name > key)
    Maybe< PropertyAttribute > GetRealNamedPropertyAttributes (Local< Context > context, Local< Name > key)
    bool HasNamedLookupInterceptor () const
    bool HasIndexedLookupInterceptor () const
    int GetIdentityHash ()
    Local< Object > Clone (v8::Isolate *isolate)
    Local< Object > Clone ()
    MaybeLocal< Context > GetCreationContext (v8::Isolate *isolate)
    MaybeLocal< Context > GetCreationContext ()
    Local< Context > GetCreationContextChecked (v8::Isolate *isolate)
    Local< Context > GetCreationContextChecked ()
    void * GetAlignedPointerFromEmbedderDataInCreationContext (v8::Isolate *isolate, int index)
    void * GetAlignedPointerFromEmbedderDataInCreationContext (int index)
    bool IsCallable () const
    bool IsConstructor () const
    bool IsApiWrapper () const
    bool IsUndetectable () const
    MaybeLocal< Value > CallAsFunction (Local< Context > context, Local< Value > recv, int argc, Local< Value > argv[])
    MaybeLocal< Value > CallAsConstructor (Local< Context > context, int argc, Local< Value > argv[])
    Isolate * GetIsolate ()
    MaybeLocal< Array > PreviewEntries (bool *is_key_value)
    bool IsCodeLike (Isolate *isolate) const
    int InternalFieldCount (const PersistentBase< Object > &object)
    static int InternalFieldCount (const BasicTracedReference< Object > &object)
    static void * GetAlignedPointerFromInternalField (const PersistentBase< Object > &object, int index)
    static void * GetAlignedPointerFromInternalField (const BasicTracedReference< Object > &object, int index)
    static void Wrap (v8::Isolate *isolate, const v8::Local< v8::Object > &wrapper, void *wrappable, CppHeapPointerTag tag)
    static void Wrap (v8::Isolate *isolate, const PersistentBase< Object > &wrapper, void *wrappable, CppHeapPointerTag tag)
    static void Wrap (v8::Isolate *isolate, const BasicTracedReference< Object > &wrapper, void *wrappable, CppHeapPointerTag tag)
    static MaybeLocal< Context > GetCreationContext (v8::Isolate *isolate, const PersistentBase< Object > &object)
    static MaybeLocal< Context > GetCreationContext (const PersistentBase< Object > &object)
    static Isolate * GetIsolate (const TracedReference< Object > &handle)
    static Local< Object > New (Isolate *isolate)
    static Local< Object > New (Isolate *isolate, Local< Value > prototype_or_null, Local< Name > *names, Local< Value > *values, size_t length)
    static Object * Cast (Value *obj)
  }
  class Primitive
  class Array{
    uint32_t Length () const
    Maybe< void > Iterate (Local< Context > context, IterationCallback callback, void *callback_data)
    static Local< Array > New (Isolate *isolate, int length=0)
    static Local< Array > New (Isolate *isolate, Local< Value > *elements, size_t length)
    static Array * Cast (Value *value)
    static MaybeLocal< Array > New (Local< Context > context, size_t length, std::function< MaybeLocal< v8::Value >()> next_value_callback)
  }
  class ArrayBuffer{
    size_t ByteLength () const
    size_t MaxByteLength () const
    bool IsDetachable () const
    bool WasDetached () const
    V8_WARN_UNUSED_RESULT Maybe< bool > Detach (v8::Local< v8::Value > key)
    void SetDetachKey (v8::Local< v8::Value > key)
    std::shared_ptr< BackingStore > GetBackingStore ()
    bool IsResizableByUserJavaScript () const
    void * Data () const
    static MaybeLocal< ArrayBuffer > MaybeNew (Isolate *isolate, size_t byte_length, BackingStoreInitializationMode initialization_mode=BackingStoreInitializationMode::kZeroInitialized)
    static Local< ArrayBuffer > New (Isolate *isolate, size_t byte_length, BackingStoreInitializationMode initialization_mode=BackingStoreInitializationMode::kZeroInitialized)
    static Local< ArrayBuffer > New (Isolate *isolate, std::shared_ptr< BackingStore > backing_store)
    static std::unique_ptr< BackingStore > NewBackingStore (Isolate *isolate, size_t byte_length, BackingStoreInitializationMode initialization_mode=BackingStoreInitializationMode::kZeroInitialized, BackingStoreOnFailureMode on_failure=BackingStoreOnFailureMode::kOutOfMemory)
    static std::unique_ptr< BackingStore > NewBackingStore (void *data, size_t byte_length, v8::BackingStore::DeleterCallback deleter, void *deleter_data)
    static std::unique_ptr< BackingStore > NewResizableBackingStore (size_t byte_length, size_t max_byte_length)
    static V8_INLINE ArrayBuffer * Cast (Value *value)
  }
  class ArrayBufferView{
    Local< ArrayBuffer > Buffer ()
    size_t ByteOffset ()
    size_t ByteLength ()
    size_t CopyContents (void *dest, size_t byte_length)
    v8::MemorySpan< uint8_t > GetContents (v8::MemorySpan< uint8_t > storage)
    bool HasBuffer () const
  }
  class BigIntObject{
    static Local< Value > New (Isolate *isolate, int64_t value)
    static BigIntObject * Cast (Value *value)
  }
  class BooleanObject{
    static Local< Value > New (Isolate *isolate, bool value)
    static BigIntObject * Cast (Value *value)
  }
  class Date{
    double ValueOf () const
    v8::Local< v8::String > ToISOString () const
    v8::Local< v8::String > ToUTCString () const
    static MaybeLocal< Value > New (Local< Context > context, double time)
    static MaybeLocal< Value > Parse (Local< Context > context, Local< String > date_string)
    static Date * Cast (Value *value)
  }
  class Function{
    MaybeLocal< Object > NewInstance (Local< Context > context, int argc, Local< Value > argv[]) const
    MaybeLocal< Object > NewInstance (Local< Context > context) const
    MaybeLocal< Object > NewInstanceWithSideEffectType (Local< Context > context, int argc, Local< Value > argv[], SideEffectType side_effect_type=SideEffectType::kHasSideEffect) const
    MaybeLocal< Value > Call (v8::Isolate *isolate, Local< Context > context, Local< Value > recv, int argc, Local< Value > argv[])
    MaybeLocal< Value > Call (Local< Context > context, Local< Value > recv, int argc, Local< Value > argv[])
    void SetName (Local< String > name)
    Local< Value > GetName () const
    Local< Value > GetInferredName () const
    Local< Value > GetDebugName () const
    int GetScriptLineNumber () const
    int GetScriptColumnNumber () const
    Location GetScriptLocation () const
    int GetScriptStartPosition () const
    int ScriptId () const
    Local< Value > GetBoundFunction () const
    MaybeLocal< String > FunctionProtoToString (Local< Context > context)
    bool Experimental_IsNopFunction () const
    ScriptOrigin GetScriptOrigin () const
    static MaybeLocal< Function > New (Local< Context > context, FunctionCallback callback, Local< Value > data=Local< Value >(), int length=0, ConstructorBehavior behavior=ConstructorBehavior::kAllow, SideEffectType side_effect_type=SideEffectType::kHasSideEffect)
    static Function * Cast (Value *value)
  }
  class Map{
    size_t Size () const
    void Clear ()
    MaybeLocal< Value > Get (Local< Context > context, Local< Value > key)
    MaybeLocal< Map > Set (Local< Context > context, Local< Value > key, Local< Value > value)
    Maybe< bool > Has (Local< Context > context, Local< Value > key)
    Maybe< bool > Delete (Local< Context > context, Local< Value > key)
    Local< Array > AsArray () const
  }
  class NumberObject{
    double ValueOf () const
  }
  class Promise{
    MaybeLocal< Promise > Catch (Local< Context > context, Local< Function > handler)
    MaybeLocal< Promise > Then (Local< Context > context, Local< Function > handler)
    MaybeLocal< Promise > Then (Local< Context > context, Local< Function > on_fulfilled, Local< Function > on_rejected)
    bool HasHandler () const
    Local< Value > Result ()
    PromiseState State ()
    void MarkAsHandled ()
    void MarkAsSilent ()
  }
  class Resolver{
    Local< Promise > GetPromise ()
    Maybe< bool > Resolve (Local< Context > context, Local< Value > value)
    Maybe< bool > Reject (Local< Context > context, Local< Value > value)
  }
  class Proxy{
    Local< Value > GetTarget ()
    Local< Value > GetHandler ()
    bool IsRevoked () const
    void Revoke ()
  }
  class RegExp{
    MaybeLocal< Object > Exec (Local< Context > context, Local< String > subject)
    Local< String > GetSource () const
    Flags GetFlags () const
  }
  class Set{
    size_t Size () const
    void Clear ()
    MaybeLocal< Set > Add (Local< Context > context, Local< Value > key)
    Maybe< bool > Has (Local< Context > context, Local< Value > key)
    Maybe< bool > Delete (Local< Context > context, Local< Value > key)
    Local< Array > AsArray () const
  }
  class SharedArrayBuffer{
    size_t ByteLength () const
    size_t MaxByteLength () const
    std::shared_ptr< BackingStore > GetBackingStore ()
    void * Data () const
  }
  class StringObject{
    Local< String > ValueOf () const
  }
  class SymbolObject{
    Local< Symbol > ValueOf () const
  }
  class WasmMemoryMapDescriptor
  class WasmMemoryObject
  class WasmModuleObject
  class Boolean{
    bool Value () const
  }
  class Name{
    int GetIdentityHash ()
  }
  class Numberic
  class DateView
  class TypedArray
  class BigInt64Array
  class String{
    int Length () const
    int Utf8Length (Isolate *isolate) const
    size_t Utf8LengthV2 (Isolate *isolate) const
    bool IsOneByte () const
    bool ContainsOnlyOneByte () const
    int Write (Isolate *isolate, uint16_t *buffer, int start=0, int length=-1, int options=NO_OPTIONS) const
    int WriteOneByte (Isolate *isolate, uint8_t *buffer, int start=0, int length=-1, int options=NO_OPTIONS) const
    int WriteUtf8 (Isolate *isolate, char *buffer, int length=-1, int *nchars_ref=nullptr, int options=NO_OPTIONS) const
    void WriteV2 (Isolate *isolate, uint32_t offset, uint32_t length, uint16_t *buffer, int flags=WriteFlags::kNone) const
    void WriteOneByteV2 (Isolate *isolate, uint32_t offset, uint32_t length, uint8_t *buffer, int flags=WriteFlags::kNone) const
    size_t WriteUtf8V2 (Isolate *isolate, char *buffer, size_t capacity, int flags=WriteFlags::kNone, size_t *processed_characters_return=nullptr) const
    bool IsExternal () const
    bool IsExternalTwoByte () const
    bool IsExternalOneByte () const
    Local< String > InternalizeString (Isolate *isolate)
    ExternalStringResourceBase * GetExternalStringResourceBase (v8::Isolate *isolate, Encoding *encoding_out) const
    ExternalStringResourceBase * GetExternalStringResourceBase (Encoding *encoding_out) const
    ExternalStringResource * GetExternalStringResource () const
    const ExternalOneByteStringResource * GetExternalOneByteStringResource () const
    bool MakeExternal (ExternalStringResource *resource)
    bool MakeExternal (Isolate *isolate, ExternalStringResource *resource)
    bool MakeExternal (ExternalOneByteStringResource *resource)
    bool MakeExternal (Isolate *isolate, ExternalOneByteStringResource *resource)
    bool CanMakeExternal (Encoding encoding) const
    bool StringEquals (Local< String > str) const
    static Local< String > Empty (Isolate *isolate)
    static String * Cast (v8::Data *data)
    static MaybeLocal< String > NewFromUtf8 (Isolate *isolate, const char *data, NewStringType type=NewStringType::kNormal, int length=-1)
    static MaybeLocal< String > NewFromOneByte (Isolate *isolate, const uint8_t *data, NewStringType type=NewStringType::kNormal, int length=-1)
    static MaybeLocal< String > NewFromTwoByte (Isolate *isolate, const uint16_t *data, NewStringType type=NewStringType::kNormal, int length=-1)
    static Local< String > Concat (Isolate *isolate, Local< String > left, Local< String > right)
    static MaybeLocal< String > NewExternalTwoByte (Isolate *isolate, ExternalStringResource *resource)
    static MaybeLocal< String > NewExternalOneByte (Isolate *isolate, ExternalOneByteStringResource *resource)
  }
  class Symbol
  class BigInt
  class Number

  CompileHintsCollector --|> Data
  Context --|> Data
  FixedArray --|> Data
  Module --|> Data
  ModuleRequest --|> Data
  PrimitiveArray --|> Data
  Private --|> Data
  Script --|> Data
  Signature --|> Data
  Template --|> Data
  UnboundModuleScript --|> Data
  UnboundScript --|> Data
  Value --|> Data
  FunctionTemplate --|> Template
  ObjectTemplate --|> Template
  External --|> Value
  Object --|> Value
  Primitive --|> Value
  Array --|> Object
  ArrayBuffer --|> Object
  ArrayBufferView --|> Object
  BigIntObject --|> Object
  BooleanObject --|> Object
  Date --|> Object
  Function --|> Object
  Map --|> Object
  NumberObject --|> Object
  Promise --|> Object
  Resolver --|> Object
  Proxy --|> Object
  RegExp --|> Object
  Set --|> Object
  SharedArrayBuffer --|> Object
  StringObject --|> Object
  SymbolObject --|> Object
  WasmMemoryMapDescriptor --|> Object
  WasmMemoryObject --|> Object
  WasmModuleObject --|> Object
  Boolean --|> Primitive
  Name --|> Primitive
  Numberic --|> Primitive
  DateView --|> ArrayBufferView
  TypedArray --|> ArrayBufferView
  BigInt64Array --|> TypedArray
  String --|> Primitive
  Symbol --|> Primitive
  BigInt --|> Primitive
  Number --|> Primitive



  note for Value "所有 JavaScript 值的基类"
  note for Object "JavaScript 对象的基础表示"
  note for FixedArray "V8 内部固定大小数组"
  note for Context "JavaScript 执行上下文"
  note for Function "JavaScript 函数对象"
  note for Promise "JavaScript Promise 对象"
  note for CompileHintsCollector "编译提示收集器"
```