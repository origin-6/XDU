#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>

#define BUF_SIZE 1024
#define DEFAULT_WIDE 16

int main(int argc, char **argv)
{
    int wide = DEFAULT_WIDE;

    printf("Path: %s\n", argv[1]);

    printf("           ");
    for (int i = 0; i < wide; ++i)
    {
        printf("%02x ", i);
    }
    printf("\n");

    // 分配大小为1024字节的内存块
    uint8_t *buf = (uint8_t *)malloc(BUF_SIZE);
    // 如果分配失败
    if (buf == NULL)
    {
        perror("malloc");
        exit(1);
    }

    // 根据输入的文件地址打开文件
    FILE *in = fopen(argv[1], "rb");
    if (in == NULL)
    {
        perror("fopen");
        free(buf);
        exit(1);
    }

    // printed 用于记录已经打印的字节数，display 用于记录打印的字符
    int printed = 0;
    char display[wide + 1];
    // 初始化 display 数组
    memset(display, 0, sizeof(display));

    // 读取文件，直到文件结束或出错
    while (!feof(in))
    {

        // 缓冲区、缓冲区单位大小（字节大小）、基本单元个数、文件指针，返回实际从文件中读取的基本单元个数
        size_t readnum = fread(buf, 1, BUF_SIZE, in);

        // 检查是否有读取错误
        if (ferror(in))
        {
            perror("fread");
            free(buf);
            fclose(in);
            exit(1);
        }

        for (size_t i = 0; i < readnum; i ++)
        {
            // 开始新的一行，打印行首地址（八位十六进制）（即总字节数）
            if (printed % wide == 0)
            {
                printf("%08x   ", printed);
            }

            // 两位十六进制打印当前字节值
            printf("%02x", buf[i]);
            // 将字节储存在 display 数组中
            display[printed % wide] = isprint(buf[i]) ? buf[i] : '.';

            // 如果不是行末，在十六进制数后加空格
            if (printed % wide != (wide - 1))
            {
                printf(" ");
            }
            // 行末
            else
            {
                display[wide] = '\0';
                printf("    %s\n", display);
                // 清空以便下次使用
                memset(display, 0, sizeof(display));
            }

            printed ++;
        }
    }

    // 打印最后一行，如果最后一行没有填满
    if (printed % wide != 0)
    {
        // 计算还缺多少字节 byte_num
        int byte_num = wide - printed % wide;
        // 用空格填满最后一行，并且打印剩下的字符
        int ws_num = 4 + 3 * byte_num - 1;
        printf("%-*s%s\n", ws_num, "", display);
    }

            fclose(in);
            free(buf);

    return 0;
}