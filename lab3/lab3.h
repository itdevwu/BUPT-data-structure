#include "./lib/huffman_tree.h"
#include "./lib/boyer_moore.h"

namespace lab3
{
    // Function for Huffman Tree building.
    bool opt1(char *corpus_file_name,
              lab3::Freq *freq_arr,
              lab3::Bitseq *coding,
              lab3::HuffmanNode **huffman_tree)
    {
        // Name / path of corpus file.
        printf("请输入语料文件的名称或路径: ");
        scanf("%s", corpus_file_name);

        FILE *corpus_file_pointer = fopen(corpus_file_name, "r+");

        // I/O error.
        if (corpus_file_pointer == nullptr)
        {
            printf("\n语料文件I/O错误! \n");
            return false;
        }

        // Read from corpus file.
        memset(freq_arr, 0, sizeof(freq_arr));
        int char_cnt = 0;

        // Count frequency.
        lab3::cnt_freq(corpus_file_pointer, freq_arr, &char_cnt);

        // Build tree.
        int *tmp = (int *)malloc(100000 * sizeof(int));
        *huffman_tree = lab3::build_huffman_tree(freq_arr, char_cnt);
        lab3::huffman_coding(*huffman_tree, 0, tmp, coding);

        // Free & return.
        fclose(corpus_file_pointer);
        free(tmp);
        return true;
    }

    // Function for compression.
    bool opt2(char *compress_file_name,
              char *corpus_file_name,
              lab3::Bitseq *coding)
    {
        printf("请输入待压缩的压缩文件的名称或路径: ");
        scanf("%s", compress_file_name);

        // File name of corpus and compression file.
        FILE *corpus_file_pointer = fopen(corpus_file_name, "r+");
        FILE *compress_file_pointer = fopen(compress_file_name, "wb+");

        // I/O errors.
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

        // Controller for bit output.
        lab3::BitIO binary_IO_controller;
        binary_IO_controller.target_file_pointer = compress_file_pointer;

        // Info of compressed bit size.
        unsigned tot_bit_cnt = 0;
        unsigned original_tot_bit_cnt = 0;
        char ll_buffer[4];

        printf("\n正在将内容压缩并存入文件: %s ...\n", compress_file_name);

        // Write blocks for validation.
        for (int i = 1; i <= 32; ++i)
            binary_IO_controller.write_bit(0);

        // Write wchar from corpus to compression file. 
        lab3::UTF8_CHAR _tmp_utf8char;
        while ((_tmp_utf8char = fgetwc(corpus_file_pointer)) != WEOF)
        {
            // Fetch coding.
            lab3::Bitseq _tmp_bitseq = coding[(unsigned)_tmp_utf8char];
            tot_bit_cnt += _tmp_bitseq.size;
            original_tot_bit_cnt += 32;
            // Output by bit.
            for (int i = 0; i < _tmp_bitseq.size; ++i)
            {
                // printf("%d", _tmp_bitseq.nth_bit(i));
                binary_IO_controller.write_bit(_tmp_bitseq.nth_bit(i));
            }
        }
        // Add 0s to the tail.
        binary_IO_controller.flush();

        // Cover validation blocks.
        memcpy(ll_buffer, &tot_bit_cnt, sizeof(unsigned));
        fseek(compress_file_pointer, 0, SEEK_SET);
        for (int i = 0; i < 4; ++i)
            fputc(ll_buffer[i], compress_file_pointer);

        // Calculate compression ratio.
        printf("\n压缩完成! 压缩文件为: %s . 压缩率为 %g %% .\n", compress_file_name, (double)(tot_bit_cnt + 32.0) / original_tot_bit_cnt * 100.0);

        fclose(compress_file_pointer);
        fclose(corpus_file_pointer);
        return true;
    }

    // Function for decompression.
    bool opt3(char *decompress_file_name,
              char *compress_file_name,
              lab3::HuffmanNode **huffman_tree)
    {
        printf("请输入待解压的压缩文件的名称或路径: ");
        scanf("%s", decompress_file_name);

        // (De)compressed file pointers.
        FILE *compress_file_pointer = fopen(compress_file_name, "r+");
        FILE *decompress_file_pointer = fopen(decompress_file_name, "w+");

        printf("%s\n", decompress_file_name);

        // I/O error.
        if (compress_file_pointer == nullptr)
        {
            printf("\n压缩文件I/O错误! \n");
            return false;
        }

        // Fetch validation blocks.
        unsigned tot_bit_cnt = 0;
        unsigned cur_bit_cnt = 0;
        char ll_buffer[4];
        for (int i = 0; i < 4; ++i)
            ll_buffer[i] = fgetc(compress_file_pointer);
        memcpy(&tot_bit_cnt, ll_buffer, sizeof(unsigned));

        // Read by bit.
        unsigned char _tmp_char;
        lab3::HuffmanNode *cur_node = *huffman_tree;
        while (1)
        {
            // Get a block.
            _tmp_char = fgetc(compress_file_pointer);
            for (int i = 0; i < 8; ++i)
            {
                ++cur_bit_cnt;

                // Move on Huffman Tree.
                int _bit = (_tmp_char >> i) & 1;
                cur_node = cur_node->iter(_bit);
                if (cur_node->is_leaf())
                {
                    // Leaf stands for actual char.
                    lab3::UTF8_CHAR _tmp_uc = cur_node->ch;
                    fputwc(cur_node->ch, decompress_file_pointer);
                    cur_node = *huffman_tree;
                }

                // Check borders by validation info instead of EOF.
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

    // Function for pattern matching.
    unsigned opt4(char *compress_file_name, lab3::Bitseq coding[])
    {
        // Store bit context of strings.
        unsigned char *pattern_bit_context = (unsigned char *)malloc(100000000 * sizeof(unsigned char *));
        unsigned char *compress_bit_context = (unsigned char *)malloc(100000000 * sizeof(unsigned char *));
        
        // File in which pattern string is stored.
        char pattern_file_name[1024];
        printf("请输入存储模式串的文件的文件名/地址(注意空行空格): ");
        scanf("%s", pattern_file_name);

        FILE *pattern_file_pointer = fopen(pattern_file_name, "r+");
        FILE *compress_file_pointer = fopen(compress_file_name, "r+");

        // I/O errors.
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

        // Read pattern bits.
        unsigned pat_pos = 0;
        lab3::UTF8_CHAR _tmp_wchar;
        while ((_tmp_wchar = fgetwc(pattern_file_pointer)) != WEOF)
        {
            // Check if the char has gotten a Huffman code.
            if (coding[(unsigned)_tmp_wchar].ch == _tmp_wchar)
            {
                auto seq = coding[(unsigned)_tmp_wchar];
                for (int i = 0; i < seq.size; ++i)
                    pattern_bit_context[pat_pos++] = '0' + seq.nth_bit(i);
            }
            else
            {
                // Non-existing char means definite failure.
                printf("未找到匹配! \n");
                return -1;
            }
        }
        // End.
        pattern_bit_context[pat_pos] = '\0';

        // Read string bits.
        // Read validation block as decompression does.
        unsigned tot_bit_cnt = 0;
        unsigned cur_bit_cnt = 0;
        char ll_buffer[4];
        for (int i = 0; i < 4; ++i)
            ll_buffer[i] = fgetc(compress_file_pointer);
        memcpy(&tot_bit_cnt, ll_buffer, sizeof(unsigned));

        // Move on tree.
        unsigned char _tmp_char;
        while (1)
        {
            // Get a block.
            _tmp_char = fgetc(compress_file_pointer);
            for (int i = 0; i < 8; ++i)
            {
                // Move on Huffman Tree.
                int _bit = (_tmp_char >> i) & 1;
                compress_bit_context[cur_bit_cnt++] = '0' + _bit;

                // Check borders by validation info instead of EOF.
                if (cur_bit_cnt >= tot_bit_cnt)
                    break;
            }
            if (cur_bit_cnt >= tot_bit_cnt)
                break;
        }
        // End.
        compress_bit_context[tot_bit_cnt] = '\0';

        printf("\n模式串的二进制形式为: %s\n\n", pattern_bit_context);

        //Matching, making sure each time match starts from larger index.
        int last_find_pos = -1;
        int next_find_pos = lab3::boyer_moore(compress_bit_context,
                                              tot_bit_cnt,
                                              pattern_bit_context,
                                              pat_pos,
                                              26);
        if (next_find_pos == -1)
        {
            // Nothing found.
            printf("未找到匹配! \n");
            return -1;
        }
        else
        {
            while (1)
            {
                // Check if this match has found new things on previous basis.
                last_find_pos = next_find_pos;
                printf("在二进制串从0开始的第 %d 位匹配! \n", last_find_pos);
                next_find_pos = last_find_pos + 1 + lab3::boyer_moore(compress_bit_context + last_find_pos + 1, tot_bit_cnt - (last_find_pos + 1), pattern_bit_context, pat_pos, 26);
                // This time ends up nothing, then break.
                if (next_find_pos - (last_find_pos + 1) == -1)
                    break;
            }
        }
        return 1;
    }
}