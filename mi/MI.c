#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_FILE        64
#define FILENAME_INPUT  "./mi_src.txt"
#define FILENAME_OUTPUT "./mi_%d.bin"

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

void parse(unsigned char *data, int pos, char num)
{
    if ('0' == num)
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
    FILE *fp = fopen(FILENAME_INPUT, "rb");

    if (NULL == fp)
    {
        printf("open file %s fail\n", FILENAME_INPUT);
        return 0;
    }

    int i;
    int j;
    int num;
    int mi_len;
    int id_len;
    int next_len;
    int line_len;
    int head_len;
    int data_len;
    int file_count;
    int id_num;
    int id_num_last;
    char id[16];
    char next[16];
    char buff[1024];
    char filename[512];
    unsigned char mi;
    unsigned char *data = NULL;
    FILE *file[MAX_FILE] = { NULL };

    mi = 0;
    id_num_last = 0;

    while (NULL != fgets(buff, sizeof(buff) - 1, fp))
    {
        sscanf(buff, "%s\t%s\t", id, next);

        id_num = 0;
        id_len = strlen(id);
        next_len = strlen(next);
        line_len = strlen(buff);
        head_len = id_len + next_len + 2; // 2个tab
        data_len = line_len - head_len;
        mi_len = (next_len - 2) * 4 + data_len / 2;
        file_count = ceil(mi_len / 8.0);

        if (file_count > MAX_FILE)
        {
            printf("file_count > %d\n", MAX_FILE);
            return 0;
        }

        if (NULL == data)
        {
            data = (unsigned char*)malloc(file_count);
        }

        memset(data, 0, file_count);

        for (i = 0; i < file_count; i++)
        {
            sprintf(filename, FILENAME_OUTPUT, i);

            if (NULL == file[i])
            {
                file[i] = fopen(filename, "wb");
            }

            if (NULL == file[i])
            {
                printf("create file %s fail\n", filename);
                return 0;
            }
        }

        for (i = 2; i < id_len; i++)
        {
            id_num = (id_num << 4) | get_hex(id[i]);
        }

        for (i = 2; i < next_len; i++)
        {
            num = get_hex(next[next_len - i + 1]);

            for (j = 0; j < 4; j++)
            {
                parse(data, (i - 2) * 4 + j, '0' + ((num >> j) & 0x01));
            }
        }

        // 填充空白
        for (i = id_num_last + 1; i < id_num; i++)
        {
            for (j = 0; j < file_count; j++)
            {
                fwrite(&mi, 1, 1, file[j]);
            }
        }

        printf("%s %s\n", id, next);

        // 解析数据
        for (i = 0; i < data_len; i++)
        {
            parse(data, (next_len - 2) * 4 + i, buff[head_len + i * 2]);
        }

        // 写入文件
        for (i = 0; i < file_count; i++)
        {
            fwrite(&data[i], 1, 1, file[i]);
        }

        id_num_last = id_num;
    }

    for (i = 0; i < file_count; i++)
    {
        fclose(file[i]);
    }

    fclose(fp);

    return 0;
}

