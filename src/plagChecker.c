#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "Word.h"
#include "LinkedList.h"

#define DEBUG 0
#define REMOVE_STOP_WORDS 1

const char *stop_words[] = {"a", "an", "the", "of", "or", "if", "on", "for"};
#define total_stop_words (sizeof(stop_words) / sizeof(const char *))

bool is_stop_word(char *word_text)
{
    for (int i = 0; i < total_stop_words; i++)
    {
        if (strcmp(word_text, stop_words[i]) == 0)
        {
            return true;
        }
    }
    return false;
}

// Find word in given list
Word *find_word(LinkedList *word_list, char *word_text)
{
    if (word_list->head == NULL)
    {
        return NULL;
    }

    ll_Node *current = word_list->head;

    while (strcmp(((Word *)current->data)->word_text, word_text) != 0)
    {
        if (current->next == NULL)
        {
            return NULL;
        }
        else
        {
            current = current->next;
        }
    }

    return ((Word *)current->data);
}

// Print word list
void print_word_list(LinkedList *word_list)
{
    ll_Node *current = word_list->head;

    while (current != NULL)
    {
        char *word_text = ((Word *)current->data)->word_text;
        int frequency = ((Word *)current->data)->frequency;
        printf("Word: %s %d\n", word_text, frequency);
        current = current->next;
    }
}

// Cosine similarity
double cosine_similarity(int vector1[], int vector2[], int size)
{
    double dot_product = 0;
    double mod1 = 0;
    double mod2 = 0;

    for (int i = 0; i < size; i++)
    {
        dot_product += vector1[i] * vector2[i];
        mod1 += vector1[i] * vector1[i];
        mod2 += vector2[i] * vector2[i];
    }

#if DEBUG
    printf("%f %f %f\n", dot_product, mod1, mod2);
#endif

    return (dot_product) / (sqrt(mod1 * mod2));
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("Enter all argumentes\n");
        return 0;
    }

    char *test_file = argv[1];
    char *corpus = argv[2];
    char *file2 = argv[2];

    FILE *test_file_ptr = fopen(test_file, "r");
    if (test_file_ptr == NULL)
    {
        printf("Unable to read test file\n");
        return 0;
    }

    LinkedList *all_words_list = ll_new_list(sizeof(Word));
    LinkedList *word_list_1 = ll_new_list(sizeof(Word));
    LinkedList *word_list_2 = ll_new_list(sizeof(Word));

    char word_text[MAX_WORD_SIZE];
    while (fscanf(test_file_ptr, " %1023s", word_text) == 1)
    {
        int j = 0;
        for (int i = 0; word_text[i]; i++)
        {
            if (word_text[i] != '.' && word_text[i] != ',')
            {
                word_text[j] = tolower(word_text[i]);
                j++;
            }
        }

        // check for empty words
        if (j == 0)
        {
            continue;
        }

        word_text[j] = '\0';
        Word *find = find_word(word_list_1, word_text);
        if (find == NULL)
        {
            Word *word = new_word(word_text);
            ll_append(word_list_1, (void *)word);
        }
        else
        {
            find->frequency++;
        }

        Word *find2 = find_word(all_words_list, word_text);
        if (find2 == NULL)
        {
            Word *word = new_word(word_text);
            ll_append(all_words_list, (void *)word);
        }
        else
        {
            find2->frequency++;
        }
    }
    fclose(test_file_ptr);

    FILE *file_ptr2 = fopen(file2, "r");
    if (file_ptr2 == NULL)
    {
        printf("Unable to read test file 2\n");
        return 0;
    }

    while (fscanf(file_ptr2, " %1023s", word_text) == 1)
    {
        int j = 0;
        for (int i = 0; word_text[i]; i++)
        {
            if (word_text[i] != '.' && word_text[i] != ',')
            {
                word_text[j] = tolower(word_text[i]);
                j++;
            }
        }

        // check for empty words
        if (j == 0)
        {
            continue;
        }

        word_text[j] = '\0';
        Word *find = find_word(word_list_2, word_text);
        if (find == NULL)
        {
            Word *word = new_word(word_text);
            ll_append(word_list_2, (void *)word);
        }
        else
        {
            find->frequency++;
        }

        Word *find2 = find_word(all_words_list, word_text);
        if (find2 == NULL)
        {
            Word *word = new_word(word_text);
            ll_append(all_words_list, (void *)word);
        }
        else
        {
            find2->frequency++;
        }
    }
    fclose(file_ptr2);

#if DEBUG
    printf("list 1\n");
    print_word_list(word_list_1);

    printf("list 2\n");
    print_word_list(word_list_2);

    printf("list all\n");
    print_word_list(all_words_list);
#endif

    int total_words = all_words_list->length;
    int word_vector_1[total_words];
    int word_vector_2[total_words];

    // for all words:
    ll_Node *current = all_words_list->head;

    int non_stop_words = 0;
    while (current != NULL)
    {
        char *word_text = ((Word *)current->data)->word_text;

#if REMOVE_STOP_WORDS
        bool take_word = !is_stop_word(word_text);
#else
        bool take_word = true;
#endif

        if (take_word)
        {
            Word *word1 = find_word(word_list_1, word_text);
            if (word1 != NULL)
            {
                word_vector_1[non_stop_words] = word1->frequency;
            }
            else
            {
                word_vector_1[non_stop_words] = 0;
            }

            Word *word2 = find_word(word_list_2, word_text);
            if (word2 != NULL)
            {
                word_vector_2[non_stop_words] = word2->frequency;
            }
            else
            {
                word_vector_2[non_stop_words] = 0;
            }

            non_stop_words++;
        }

        current = current->next;
    }

#if DEBUG
    printf("Word vector size: %d\n", non_stop_words);

    for (int i = 0; i < non_stop_words; i++)
    {
        printf("%d %d\n", word_vector_1[i], word_vector_2[i]);
    }
#endif

    double similarity = cosine_similarity(word_vector_1, word_vector_2, non_stop_words);
    printf("%f\n", similarity);

    // Freeing
    while (!ll_is_empty(word_list_1))
    {
        ll_remove_head(word_list_1);
    }
    free(word_list_1);
    
    while (!ll_is_empty(word_list_2))
    {
        ll_remove_head(word_list_2);
    }
    free(word_list_2);
    
    while (!ll_is_empty(all_words_list))
    {
        ll_remove_head(all_words_list);
    }
    free(all_words_list);

    return 0;
}