/*
 * A284894  Positions of 0 in A284893; complement of A284895.
 * https://oeis.org/A284894
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 76, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: generate the infinite morphic word defined by
   sigma(0)=01, sigma(1)=0111.  Return the positions of 0 in
   that word (1‑based).  The function builds successive
   words until enough 0‑positions have been collected. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* The function prototype is supplied by the build.  It must not
   be declared static.  The types seq_t and idx_t are provided
   by the build system. */
seq_t a(idx_t n)
{
    /* static state across calls ------------------------------------- */
    static unsigned char *word = NULL;          /* current word        */
    static size_t word_len = 0;                 /* length of word      */
    static seq_t *pos_arr = NULL;               /* array of zero positions */
    static size_t pos_len = 0;                  /* number of positions stored */
    static size_t pos_cap = 0;                  /* capacity of pos_arr  */

    /* initialise on first call ------------------------------------- */
    if (word_len == 0)
    {
        word_len = 1;
        word = (unsigned char *)malloc(sizeof(unsigned char));
        word[0] = 0;                            /* start with "0" */

        /* compute initial positions array */
        pos_cap = 1;
        pos_arr = (seq_t *)malloc(pos_cap * sizeof(seq_t));
        pos_len = 1;
        pos_arr[0] = 1;                         /* position of the
                                                   first (and only)
                                                   zero in the word   */
    }

    /* if we already have enough zero positions, return the answer */
    if (n <= pos_len)
        return pos_arr[n - 1];

    /* grow the word until enough zeros are found ------------------- */
    while (pos_len < n)
    {
        /* compute length of the next generated word ---------------- */
        size_t new_len = 0;
        for (size_t i = 0; i < word_len; ++i)
            new_len += (word[i] == 0 ? 2 : 4);

        /* allocate and fill the next word ------------------------ */
        unsigned char *next = (unsigned char *)malloc(new_len);
        size_t idx = 0;
        for (size_t i = 0; i < word_len; ++i)
        {
            if (word[i] == 0) {          /* sigma(0) = 01 */
                next[idx++] = 0;
                next[idx++] = 1;
            } else {                     /* sigma(1) = 0111 */
                next[idx++] = 0;
                next[idx++] = 1;
                next[idx++] = 1;
                next[idx++] = 1;
            }
        }

        /* replace old word with the new one --------------------- */
        free(word);
        word = next;
        word_len = new_len;

        /* recompute zero positions for the new word --------------- */
        pos_len = 0;
        seq_t pos = 1;                    /* positions are 1‑based */
        for (size_t i = 0; i < word_len; ++i)
        {
            if (word[i] == 0)
            {
                if (pos_len == pos_cap)
                {
                    size_t new_cap = pos_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(pos_arr, new_cap * sizeof(seq_t));
                    /* errno is ignored – assume allocation succeeds */
                    pos_arr = tmp;
                    pos_cap = new_cap;
                }
                pos_arr[pos_len++] = pos;
            }
            ++pos;
        }
        /* loop may repeat if still not enough zeros */
    }

    /* return the required term */
    return pos_arr[n - 1];
}
