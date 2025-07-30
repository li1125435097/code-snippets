# 编译器设置
CC := gcc
CXX := g++
CFLAGS := -Wall -Wextra -O2 -g
CXXFLAGS := -Wall -Wextra -O2 -g -std=c++17

# 项目设置
TARGET := myapp
BUILD_DIR := build
SRC_DIRS := src
INC_DIRS := include

# 自动获取所有源文件
SRCS := $(shell find $(SRC_DIRS) -name '*.c' -or -name '*.cpp')
# 生成对应的目标文件 (.o)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
# 生成依赖文件 (.d)
DEPS := $(OBJS:.o=.d)

# 包含目录
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

# 编译选项
CPPFLAGS := $(INC_FLAGS) -MMD -MP

# 默认目标
all: $(BUILD_DIR)/$(TARGET)

# 主目标：链接可执行文件
$(BUILD_DIR)/$(TARGET): $(OBJS)
	@echo "Linking $@..."
	@mkdir -p $(@D)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

# 编译C++源文件
$(BUILD_DIR)/%.cpp.o: %.cpp
	@echo "Compiling $<..."
	@mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

# 编译C源文件
$(BUILD_DIR)/%.c.o: %.c
	@echo "Compiling $<..."
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# 包含自动生成的依赖关系
-include $(DEPS)

# 清理构建产物
clean:
	@echo "Cleaning..."
	rm -rf $(BUILD_DIR)

# 安装到系统目录 (需要sudo)
install: $(BUILD_DIR)/$(TARGET)
	@echo "Installing to /usr/local/bin"
	install $< /usr/local/bin/$(TARGET)

# 运行程序
run: all
	@echo "Running $(TARGET)..."
	./$(BUILD_DIR)/$(TARGET)

# 显示帮助信息
help:
	@echo "Makefile 命令:"
	@echo "  make       - 编译项目"
	@echo "  make clean - 清理构建产物"
	@echo "  make run   - 编译并运行程序"
	@echo "  make install - 安装到系统目录"
	@echo "  make help  - 显示此帮助"

.PHONY: all clean install run help