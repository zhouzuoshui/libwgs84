# Makefile

# 编译器
CC = gcc

# 编译选项
CFLAGS = -Wall -Wextra -O2 -fPIC  # -fPIC 是生成动态库的关键
CPPFLAGS = -Iinclude  # 指定头文件路径

# 源文件目录和对象文件
SRCDIR = src
OBJDIR = obj
SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# 动态库名称（跨平台）
ifeq ($(OS),Windows_NT)
    # Windows (MSYS2/Cygwin)
    LIBNAME = libwgs84.dll
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Darwin)
        # macOS
        LIBNAME = libwgs84.dylib
    else
        # Linux, BSD, etc.
        LIBNAME = libwgs84.so
    endif
endif

# 默认目标
.PHONY: all clean

all: $(LIBNAME)

# 创建 obj 目录并编译目标文件
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# 创建 obj 目录
$(OBJDIR):
	mkdir -p $(OBJDIR)

# 生成动态库
$(LIBNAME): $(OBJECTS)
	$(CC) -shared -o $@ $^

# 清理
clean:
	rm -rf $(OBJDIR) $(LIBNAME)

# 可选：安装（例如到 /usr/local/lib）
install: $(LIBNAME)
	install -m 755 $(LIBNAME) /usr/local/lib/
	install -m 644 include/*.h /usr/local/include/

# 可选：卸载
uninstall:
	rm -f /usr/local/lib/$(LIBNAME)
	rm -f /usr/local/include/math_utils.h /usr/local/include/string_utils.h