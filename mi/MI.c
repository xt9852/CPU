#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_FILE        64
#define FILENAME_INPUT  "./mi_src.txt"
#define FILENAME_OUTPUT "./mi_%d.bin"

// 将16进制字符串转成数字
unsigned char get_hex(char i)
{
    if (i >= '0' && i <= '9')
    {
        return i - '0';
    }
    else if (i >= 'a' && i <= 'f')
    {
        return i - 'a' + 10;
    }
    else if (i >= 'A' && i <= 'F')
    {
        return i - 'A' + 10;
    }
    else
    {
        printf("%s %c error\n", __FUNCTION__, i);
        return 0;
    }
}

// 解析MI,data为数据,pos为位置,num为数值0或1
void parse(unsigned char *data, int pos, char num)
{
    if ('0' == num || 0 == num)
    {
        return;
    }

    int bit_id = pos % 8;
    int data_id = pos / 8;

    unsigned char item = 0x01;

    item <<= bit_id;

    data[data_id] |= item;
}

int main()
{
    FILE *fp = NULL;

    fopen_s(&fp, FILENAME_INPUT, "rb");

    if (NULL == fp)
    {
        printf("open file %s fail\n", FILENAME_INPUT);
        return 0;
    }

    int i;
    int j;
    int num;
    int id_len;
    int next_len;
    int line_len;
    int head_len;
    int data_len;
    int file_cnt;
    int id_num;
    int id_num_last = 0;
    int first = 0;
    char id[16];
    char next[16];
    char buff[1024];
    char filename[512];
    unsigned char mi = 0;
    unsigned char *data = NULL;
    FILE *file[MAX_FILE] = { NULL };

    while (NULL != fgets(buff, sizeof(buff) - 1, fp))
    {
        sscanf_s(buff, "%s\t%s\t", id, sizeof(id), next, sizeof(next));

        if (0 == first)
        {
            first = 1;
            id_len = strlen(id);                    // id字段长
            next_len = strlen(next);                // next字段长
            line_len = strlen(buff);                // 全行数据部长
            head_len = id_len + next_len + 2;       // 2个tab
            data_len = (line_len - head_len) / 2;   // 微指令个数
            file_cnt = (int)ceil(((next_len - 2) * 4 + data_len) / 8.0);   // 文件个数,上取整

            printf("id_len:%d\n", id_len);
            printf("next_len:%d\n", next_len);
            printf("data_len:%d\n", data_len);
            printf("line_len:%d\n", line_len);
            printf("file_cnt:%d\n", file_cnt);

            if (file_cnt > MAX_FILE)
            {
                printf("file_count > %d\n", MAX_FILE);
                return 0;
            }

            data = (unsigned char*)malloc(file_cnt + 1);

            if (NULL == data)
            {
                printf("malloc file buff fail\n");
                return 0;
            }

            for (i = 0; i < file_cnt; i++)
            {
                sprintf_s(filename, sizeof(filename) - 1, FILENAME_OUTPUT, i);

                if (NULL == file[i])
                {
                    fopen_s(&file[i], filename, "wb");
                }

                if (NULL == file[i])
                {
                    printf("create file %s fail\n", filename);
                    return 0;
                }
            }
        }

        id_num = 0;
        memset(data, 0, file_cnt);

        for (i = 2; i < id_len; i++) // 前部为"0x"
        {
            id_num = (id_num << 4) | get_hex(id[i]); // 转成数字
        }

        for (i = 2; i < next_len; i++) // 解析next,前部为"0x"
        {
            num = get_hex(next[next_len - i + 1]); // 到序取每1位的数值

            for (j = 0; j < 4; j++) // 取每1bit
            {
                parse(data, (i - 2) * 4 + j, ((num >> j) & 0x01));
            }
        }

        // 填充空白
        for (i = id_num_last + 1; i < id_num; i++)
        {
            for (j = 0; j < file_cnt; j++)
            {
                fwrite(&mi, 1, 1, file[j]);
            }
        }

        printf("%s %s 0x%x-0x%x=%d\n", id, next, id_num, id_num_last, id_num - id_num_last - 1);

        // 解析数据
        for (i = 0; i < data_len; i++)
        {
            parse(data, (next_len - 2) * 4 + i, buff[head_len + i * 2]);
        }

        // 写入文件
        for (i = 0; i < file_cnt; i++)
        {
            fwrite(&data[i], 1, 1, file[i]);
        }

        id_num_last = id_num;
    }

    for (i = 0; i < file_cnt; i++)
    {
        fclose(file[i]);
    }

    fclose(fp);

    if (NULL != data)
    {
        free(data);
    }

    return 0;
}

