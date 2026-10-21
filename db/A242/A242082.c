/*
 * A242082  Nim sequence of game on n counters whose legal moves are removing
 *          some number of counters in A027941.
 * https://oeis.org/A242082
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence generated iteratively as described in the OEIS entry for A242082. */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static data that persists across calls */
    static seq_t **words = NULL;   /* array of pointers to each w_i word */
    static idx_t *len_word = NULL; /* length of each word */
    static size_t word_cnt = 0;    /* number of words stored */
    static size_t words_cap = 0;   /* capacity of the words array */

    /* initialize on first call */
    if (word_cnt == 0)
    {
        words_cap = 8;
        words = (seq_t **)malloc(words_cap * sizeof(seq_t *));
        len_word = (idx_t *)malloc(words_cap * sizeof(idx_t));

        /* w1 = "01" */
        seq_t *w1 = (seq_t *)malloc(2 * sizeof(seq_t));
        w1[0] = (seq_t)0;
        w1[1] = (seq_t)1;
        words[0] = w1;
        len_word[0] = 2;
        word_cnt = 1;
    }

    /* extend until we have a word long enough to answer the query */
    while (len_word[word_cnt - 1] <= n)
    {
        /* compute length of the new word w_{new} */
        idx_t new_len = 2 * len_word[word_cnt - 1];           /* two copies of w_{prev} */
        for (size_t j = word_cnt - 2; j < word_cnt; --j)     /* words w_{prev-1} ... w1 */
            new_len += len_word[j];
        new_len += 1;                                        /* final "2" */

        /* allocate storage for the new word */
        seq_t *new_word = (seq_t *)malloc(new_len * sizeof(seq_t));
        size_t pos = 0;

        /* copy first two copies of w_{prev} */
        memcpy(new_word + pos, words[word_cnt - 1], len_word[word_cnt - 1] * sizeof(seq_t));
        pos += len_word[word_cnt - 1];
        memcpy(new_word + pos, words[word_cnt - 1], len_word[word_cnt - 1] * sizeof(seq_t));
        pos += len_word[word_cnt - 1];

        /* copy remaining words in reverse order */
        for (size_t j = word_cnt - 2; j < word_cnt; --j)
        {
            memcpy(new_word + pos, words[j], len_word[j] * sizeof(seq_t));
            pos += len_word[j];
        }

        /* append the final '2' */
        new_word[pos] = (seq_t)2;
        ++pos;  /* pos should now == new_len */

        /* reallocate arrays if needed */
        if (word_cnt >= words_cap)
        {
            words_cap *= 2;
            words = (seq_t **)realloc(words, words_cap * sizeof(seq_t *));
            len_word = (idx_t *)realloc(len_word, words_cap * sizeof(idx_t));
        }

        /* store the new word */
        words[word_cnt] = new_word;
        len_word[word_cnt] = new_len;
        ++word_cnt;
    }

    /* return the requested term */
    return words[word_cnt - 1][n];
}
