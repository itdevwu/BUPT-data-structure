#include "./types.h"

namespace lab3
{
    struct HuffmanNode
    {
        // Char of this node, remains empty except this is a leaf.
        lab3::UTF8_CHAR ch;
        // Weight of Huffman Tree is frequency.
        int weight;

        // Tree structure.
        HuffmanNode *parent, *lchild, *rchild;

        HuffmanNode *iter(int x)
        {
            if (x == 0)
                return lchild;
            if (x == 1)
                return rchild;
            return nullptr;
        }

        bool is_leaf()
        {
            return (lchild == nullptr && rchild == nullptr);
        }

        HuffmanNode()
        {
            parent = nullptr;
            lchild = nullptr;
            rchild = nullptr;
        }
    };

    // Build a Huffman Tree based on a given corpus.
    HuffmanNode *build_huffman_tree(Freq freq_arr[], int char_cnt)
    {
        HuffmanNode **huffman_forest;
        huffman_forest = (HuffmanNode **)malloc(100000 * sizeof(HuffmanNode *));
        HuffmanNode *root = nullptr;

        // Enumerate each char.
        for (int i = 0; i < char_cnt; ++i)
        {
            HuffmanNode *tmp_tree = nullptr;
            tmp_tree = (HuffmanNode *)malloc(sizeof(HuffmanNode));
            tmp_tree->weight = freq_arr[i].frequency;
            tmp_tree->ch = freq_arr[i].content;
            tmp_tree->lchild = tmp_tree->rchild = nullptr;
            huffman_forest[i] = tmp_tree;
        }

        for (int i = 1; i < char_cnt; ++i)
        {
            int minn = -1, minnSub;
            for (int j = 0; j < char_cnt; ++j)
            {
                if (huffman_forest[j] != nullptr && minn == -1)
                {
                    minn = j;
                    continue;
                }
                if (huffman_forest[j] != nullptr)
                {
                    minnSub = j;
                    break;
                }
            }

            for (int j = minnSub; j < char_cnt; ++j)
            {
                if (huffman_forest[j] != nullptr)
                {
                    if (huffman_forest[j]->weight < huffman_forest[minn]->weight)
                    {
                        minnSub = minn;
                        minn = j;
                    }
                    else if (huffman_forest[j]->weight < huffman_forest[minnSub]->weight)
                    {
                        minnSub = j;
                    }
                }
            }

            // 建新树
            root = (HuffmanNode *)malloc(sizeof(HuffmanNode));
            root->weight = huffman_forest[minn]->weight + huffman_forest[minnSub]->weight;
            root->lchild = huffman_forest[minn];
            root->rchild = huffman_forest[minnSub];

            huffman_forest[minn] = root;
            huffman_forest[minnSub] = nullptr;
        }

        return root;
    }

    void huffman_coding(HuffmanNode *root, int max_coding_length, int arr[], Bitseq coding[])
    {
        if (root != nullptr)
        {
            if (root->is_leaf())
            {
                printf("频率为 %d , 编码为 0x%x 的字符的 Huffman 编码为: ", root->weight, root->ch);
                for (int i = 0; i < max_coding_length; ++i)
                    printf("%d ", arr[i]);
                printf("\n");
                coding[root->ch].data = 0;
                coding[root->ch].size = 0;
                coding[root->ch].ch = root->ch;
                for (int i = max_coding_length - 1; i >= 0; --i)
                {
                    ++coding[root->ch].size;
                    coding[root->ch].data <<= 1;
                    coding[root->ch].data |= arr[i];
                }
            }
            else
            {
                arr[max_coding_length] = 0;
                huffman_coding(root->lchild, max_coding_length + 1, arr, coding);
                arr[max_coding_length] = 1;
                huffman_coding(root->rchild, max_coding_length + 1, arr, coding);
            }
        }
    }

    int exists_char(lab3::UTF8_CHAR ch, Freq *freq_arr, int max_size)
    {
        for (int i = 0; i < max_size; ++i)
        {
            if (freq_arr[i].content == ch)
            {
                return i;
            }
        }
        return -1;
    }

    void cnt_freq(FILE *corpus_file_pointer, Freq *freq_arr, int *char_cnt)
    {
        lab3::UTF8_CHAR _tmp_char;
        while ((_tmp_char = fgetwc(corpus_file_pointer)) != WEOF)
        {
            int char_index = exists_char(_tmp_char, freq_arr, *char_cnt);
            if (char_index != -1)
            {
                ++freq_arr[char_index].frequency;
            }
            else
            {
                freq_arr[*char_cnt].content = _tmp_char;
                freq_arr[*char_cnt].frequency = 1;
                *char_cnt += 1;
            }
        }
    }

    void show_corpus(Freq *corpus_freq, int size)
    {
        printf("|----------|-----------|\n");
        printf("|  CODING  | FREQUENCY |\n");
        printf("|----------|-----------|\n");
        for (int i = 0; i < size; ++i)
        {
            printf("|  \'0x%x\'%s  |     %d%s  |\n", (int)corpus_freq[i].content, ((int)corpus_freq[i].content < 10 ? "   " : ((int)corpus_freq[i].content < 100 ? "  " : " ")), corpus_freq[i].frequency, (corpus_freq[i].frequency < 10 ? "   " : (corpus_freq[i].frequency < 100 ? "  " : " ")));
            printf("|----------|-----------|\n");
        }
        printf("\n");
    }
}