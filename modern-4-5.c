#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#define BUF_SIZE 4096 /* 使用一个4096字节大小的缓冲区 */
#define OUTPUT_MODE 0700 /* 输出文件的保护位 */

int main(int argc, char *argv[])
{
    int in_fd, out_fd, rd_count, wt_count;
    char buffer[BUF_SIZE];

    if (argc != 3)
        exit(1);

    /* 打开输入文件并创建输出文件 */
    in_fd = open(argv[1], O_RDONLY); /* 打开源文件 */
    if (in_fd < 0)
        exit(2); /* 如果该文件不能打开，退出 */
    out_fd = creat(argv[2], OUTPUT_MODE); /* 创建目标文件 */
    if (out_fd < 0)
        exit(3); /* 如果该文件不能创建，退出 */

    /* 循环复制 */
    while (1)
    {
        rd_count = read(in_fd, buffer, BUF_SIZE); /* 读一块数据 */
        if (rd_count <= 0)
            break; /* 如果文件结束或者读时出错，退出循环 */
        wt_count = write(out_fd, buffer, rd_count); /* 写数据 */
        if (wt_count <= 0)
            exit(4); /* wt_count <= 0是一个错误 */
    }

    /* 关闭文件 */
    close(in_fd);
    close(out_fd);
    if (rd_count == 0) /* 没有读取错误 */
        exit(0);
    else
        exit(5); /* 有读取错误发生 */
}