#include "./lib/huffman_tree.h"
#include "./lib/boyer_moore.h"

namespace lab3
{
    bool opt1(char *corpus_file_name, lab3::Freq *freq_arr, lab3::Bitseq *coding, lab3::HuffmanNode **huffman_tree)
    {
        // Name / path of corpus file.
        printf("请输入语料文件的名称或路径: ");
        scanf("%s", corpus_file_name);

        FILE *corpus_file_pointer = fopen(corpus_file_name, "r+");

        if (corpus_file_pointer == nullptr)
        {
            printf("\n语料文件I/O错误! \n");
            return false;
        }

        // Read from corpus file.
        memset(freq_arr, 0, sizeof(freq_arr));
        int char_cnt = 0;

        lab3::cnt_freq(corpus_file_pointer, freq_arr, &char_cnt);

        show_corpus(freq_arr, char_cnt);

        int *tmp = (int *)malloc(100000 * sizeof(int));
        *huffman_tree = lab3::build_huffman_tree(freq_arr, char_cnt);
        lab3::huffman_coding(*huffman_tree, 0, tmp, coding);

        fclose(corpus_file_pointer);
        free(tmp);
        return true;
    }

    bool opt2(char *compress_file_name, char *corpus_file_name, lab3::Bitseq *coding)
    {
        printf("请输入待压缩的压缩文件的名称或路径: ");
        scanf("%s", compress_file_name);

        FILE *corpus_file_pointer = fopen(corpus_file_name, "r+");
        FILE *compress_file_pointer = fopen(compress_file_name, "wb+");

        if (corpus_file_pointer == nullptr)
        {
            printf("\n语料文件I/O错误! \n");
            return false;
        }
        if (compress_file_pointer == nullptr)
        {
            printf("\n写入目标文件I/O错误! \n");
            return false;
        }

        lab3::BitIO binary_IO_controller;
        binary_IO_controller.target_file_pointer = compress_file_pointer;

        unsigned tot_bit_cnt = 0;
        unsigned original_tot_bit_cnt = 0;
        char ll_buffer[4];

        printf("\n正在将内容压缩并存入文件: %s ...\n", compress_file_name);

        for (int i = 1; i <= 32; ++i)
            binary_IO_controller.write_bit(0);

        lab3::UTF8_CHAR _tmp_utf8char;
        while ((_tmp_utf8char = fgetwc(corpus_file_pointer)) != WEOF)
        {
            lab3::Bitseq _tmp_bitseq = coding[(unsigned)_tmp_utf8char];
            tot_bit_cnt += _tmp_bitseq.size;
            original_tot_bit_cnt += 8;
            for (int i = 0; i < _tmp_bitseq.size; ++i)
            {
                // printf("%d", _tmp_bitseq.nth_bit(i));
                binary_IO_controller.write_bit(_tmp_bitseq.nth_bit(i));
            }
        }
        binary_IO_controller.flush();

        memcpy(ll_buffer, &tot_bit_cnt, sizeof(unsigned));
        fseek(compress_file_pointer, 0, SEEK_SET);
        for (int i = 0; i < 4; ++i)
            fputc(ll_buffer[i], compress_file_pointer);

        printf("\n压缩完成! 压缩文件为: %s . 压缩率为 %g %% .\n", compress_file_name, (double)(tot_bit_cnt + 32.0) / original_tot_bit_cnt * 100.0);

        fclose(compress_file_pointer);
        fclose(corpus_file_pointer);
        return true;
    }

    bool opt3(char *decompress_file_name, char *compress_file_name, lab3::HuffmanNode **huffman_tree)
    {
        printf("请输入待解压的压缩文件的名称或路径: ");
        scanf("%s", decompress_file_name);

        FILE *compress_file_pointer = fopen(compress_file_name, "r+");
        FILE *decompress_file_pointer = fopen(decompress_file_name, "w+");

        printf("%s\n", decompress_file_name);

        if (compress_file_pointer == nullptr)
        {
            printf("\n压缩文件I/O错误! \n");
            return false;
        }

        unsigned tot_bit_cnt = 0;
        unsigned cur_bit_cnt = 0;
        char ll_buffer[4];
        for (int i = 0; i < 4; ++i)
            ll_buffer[i] = fgetc(compress_file_pointer);
        memcpy(&tot_bit_cnt, ll_buffer, sizeof(unsigned));

        unsigned char _tmp_char;
        lab3::HuffmanNode *cur_node = *huffman_tree;
        while (1)
        {
            _tmp_char = fgetc(compress_file_pointer);
            for (int i = 0; i < 8; ++i)
            {
                ++cur_bit_cnt;

                int _bit = (_tmp_char >> i) & 1;
                cur_node = cur_node->iter(_bit);
                if (cur_node->is_leaf())
                {
                    lab3::UTF8_CHAR _tmp_uc = cur_node->ch;
                    fputwc(cur_node->ch, decompress_file_pointer);
                    cur_node = *huffman_tree;
                }

                if (cur_bit_cnt >= tot_bit_cnt)
                    break;
            }
            if (cur_bit_cnt >= tot_bit_cnt)
                break;
        }

        fclose(decompress_file_pointer);
        fclose(compress_file_pointer);

        return true;
    }

    unsigned opt4(char *compress_file_name, lab3::Bitseq coding[])
    {
        unsigned char *pattern_bit_context = (unsigned char *)malloc(100000000 * sizeof(unsigned char *));
        unsigned char *compress_bit_context = (unsigned char *)malloc(100000000 * sizeof(unsigned char *));
        char pattern_file_name[1024];
        printf("请输入存储模式串的文件的文件名/地址(注意空行空格): ");
        scanf("%s", pattern_file_name);

        FILE *pattern_file_pointer = fopen(pattern_file_name, "r+");
        FILE *compress_file_pointer = fopen(compress_file_name, "r+");

        if (pattern_file_pointer == nullptr)
        {
            printf("\nI/O错误! \n");
            return -1;
        }
        if (compress_file_pointer == nullptr)
        {
            printf("\nI/O错误! \n");
            return -1;
        }

        unsigned pat_pos = 0;
        lab3::UTF8_CHAR _tmp_wchar;
        while ((_tmp_wchar = fgetwc(pattern_file_pointer)) != WEOF)
        {
            if (coding[(unsigned)_tmp_wchar].ch == _tmp_wchar)
            {
                auto seq = coding[(unsigned)_tmp_wchar];
                for (int i = 0; i < seq.size; ++i)
                    pattern_bit_context[pat_pos++] = '0' + seq.nth_bit(i);
            }
            else
            {
                printf("未找到匹配! \n");
                return -1;
            }
        }
        pattern_bit_context[pat_pos] = '\0';

        unsigned tot_bit_cnt = 0;
        unsigned cur_bit_cnt = 0;
        char ll_buffer[4];
        for (int i = 0; i < 4; ++i)
            ll_buffer[i] = fgetc(compress_file_pointer);
        memcpy(&tot_bit_cnt, ll_buffer, sizeof(unsigned));

        unsigned char _tmp_char;
        while (1)
        {
            _tmp_char = fgetc(compress_file_pointer);
            for (int i = 0; i < 8; ++i)
            {
                int _bit = (_tmp_char >> i) & 1;
                compress_bit_context[cur_bit_cnt++] = '0' + _bit;

                if (cur_bit_cnt >= tot_bit_cnt)
                    break;
            }
            if (cur_bit_cnt >= tot_bit_cnt)
                break;
        }
        compress_bit_context[tot_bit_cnt] = '\0';

        printf("\n模式串的二进制形式为: %s\n\n", pattern_bit_context);

        int last_find_pos = -1;
        int next_find_pos = lab3::boyer_moore(compress_bit_context,
                                              tot_bit_cnt,
                                              pattern_bit_context,
                                              pat_pos,
                                              100000);
        if (next_find_pos == -1)
        {
            printf("未找到匹配! \n");
            return -1;
        }
        else
        {
            while (1)
            {
                last_find_pos = next_find_pos;
                printf("在二进制串从0开始的第 %d 位匹配! \n", last_find_pos);
                next_find_pos = last_find_pos + 1 + lab3::boyer_moore(
                                                        compress_bit_context + last_find_pos + 1,
                                                        tot_bit_cnt - (last_find_pos + 1),
                                                        pattern_bit_context,
                                                        pat_pos,
                                                        100000);
                if (next_find_pos - (last_find_pos + 1) == -1)
                    break;
            }
        }
        return 1;
    }
}