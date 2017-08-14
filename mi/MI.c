#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define FILENAME_INPUT  "./mi_src.txt"
#define FILENAME_OUTPUT "./mi_%d.bin"

int get_hex(char i)
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
        return -1;
    }
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
    int pos;
    int len;
    int head;
    int count;
    int out_id;
    int id_num;
    int id_num_last;
    int next_num;
    char *ptr;
    char id[16];
    char next[16];
    char buf[1024];
    char filename[64];
    unsigned char mi;
    int n[64] = { 0 };
    FILE *out[64] = { NULL };
    id_num_last = 0;

    while (NULL != fgets(buf, sizeof(buf) - 1, fp))
    {
        sscanf(buf, "%s\t%s", id, next);
        len = strlen(buf) - 1; // \n
        pos = strlen(id);
        head = pos + strlen(next) + 2;
        count = (len - head) / 16 + (((len - head) % 16) != 0);
        id_num = 0;
        next_num = 0;

        for (i = 2; i < pos; i++)
        {
            id_num = (id_num << 4) | get_hex(id[i]);
        }

        for (i = 2; i < pos; i++)
        {
            next_num = (next_num << 4) | get_hex(next[i]);
        }

        for (i = 0; i <= count; i++)
        {
            sprintf(filename, FILENAME_OUTPUT, i);

            if (NULL == out[i])
            {
                out[i] = fopen(filename, "wb");
            }

            if (NULL == out[i])
            {
                printf("create file %s fail\n", filename);
                return 0;
            }
        }

        mi = 0;
        ptr = buf + head;

        for (i = id_num_last + 1; i < id_num; i++)
        {
            for (out_id = 0; out_id <= count; out_id++)
            {
                n[out_id]++;
                fwrite(&mi, 1, 1, out[out_id]);
            }
        }

        printf("%s %s %x %x\n", id, next, id_num, next_num);

        for (out_id = 0, pos = 0; *ptr == '0' || *ptr == '1'; ptr += 2)
        {
            mi |= ((*ptr - '0') << pos);

            if (++pos == 8)
            {
                printf("out_id:%d %d\n", out_id, n[out_id]++);
                fwrite(&mi, 1, 1, out[out_id]);
                mi = 0;
                pos = 0;
                out_id++;
                
            }
        }

        if (pos != 0)
        {
            printf("out_id:%d %d\n", out_id, n[out_id]++);
            fwrite(&mi, 1, 1, out[out_id]);
        }

        fwrite(&next_num, 1, 1, out[count]);

        id_num_last = id_num;
    }

    for (i = 0; i <= count; i++)
    {
        fclose(out[i]);
    }

    fclose(fp);

    return 0;
}

