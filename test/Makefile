
# 示例：构建多个可执行文件
all: test_utm test_registration

test_utm: test_utm.c
	gcc -o test_utm test_utm.c -I../include -L.. -lwgs84 -lm

test_registration: test_registration.c
	gcc -o test_registration test_registration.c -lgsl -lgslcblas -lm


# 通用的 .o 文件规则
%.o: %.c
	gcc -c $< -o $@

# 清理目标
clean:
	rm -f *.o test_utm test_registration

.PHONY: all clean