#ifdef _WIN32
#include <windows.h>
#endif

// Lab3 header.
#include "./lab3.h"

int main(int argc, char *argv[])
{
    // Locale settings, Linux is preffered.
#ifdef __linux__
    char *locale = setlocale(LC_ALL, "");
#endif

#ifdef _WIN32
    char *locale = setlocale(LC_ALL, "zh-CN.UTF-8");
    system("chcp 65001");
#endif

    // Mark progress of the current program.
    bool has_built_huffman_tree = false;
    bool has_compressed = false;

    // Frequency & Huffman coding & Huffman Tree root.
    lab3::Freq *freq_arr = (lab3::Freq *)malloc(10000000 * sizeof(lab3::Freq));
    lab3::Bitseq *coding = (lab3::Bitseq *)malloc(10000000 * sizeof(lab3::Bitseq));
    lab3::HuffmanNode *huffman_tree = nullptr;
    
    // File names.
    char corpus_file_name[1024];
    char compress_file_name[1024];
    char decompress_file_name[1024];

    printf("Welcome to the Huffman World!\n\n");
    printf("\n请确保所有文件为 UTF-8 编码! \n\n");

    while (1)
    {
        printf("\n请按需求:\n");
        printf("[1] 从指定语料文件构建 Huffman 树（%s）\n", "可选");
        printf("[2] 压缩语料文件并存入指定文件（%s）\n", has_built_huffman_tree ? "可选" : "不可选");
        printf("[3] 解压压缩文件并存入指定文件（%s）\n", has_compressed ? "可选" : "不可选");
        printf("[4] 在压缩文件中不解压查找字符串（%s）\n", has_compressed ? "可选" : "不可选");
        printf("[5] 退出（%s）\n", "可选");
        printf("请输入您的选项: ");

        // User option handling.
        int _opt;
        scanf("%d", &_opt);

        if (_opt == 1)
        {
            // Build Huffman tree.
            has_built_huffman_tree = lab3::opt1(corpus_file_name, freq_arr, coding, &huffman_tree);
            has_compressed = false;
        }
        else if (has_built_huffman_tree && _opt == 2)
        {
            // Compress.
            has_compressed = lab3::opt2(compress_file_name, corpus_file_name, coding);
        }
        else if (has_built_huffman_tree && _opt == 3)
        {
            // Decompress.
            lab3::opt3(decompress_file_name, compress_file_name, &huffman_tree);
        }
        else if (has_compressed && _opt == 4)
        {
            // Pattern matching.
            lab3::opt4(compress_file_name, coding);
        }
        else if (_opt == 5)
        {
            // Exit.
            free(coding);
            free(freq_arr);
            free(huffman_tree);

            return 0;
        }
        else
        {
            // Retry.
            printf("\n您的输入无效! 请重新输入! \n");
            continue;
        }
    }
    
    return 0;
}