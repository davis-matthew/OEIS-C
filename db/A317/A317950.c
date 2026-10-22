/*
 * A317950  First differences of ternary tribonacci word A080843.
 * https://oeis.org/A317950
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A317950 – first differences of the ternary tribonacci word.
 * Let w[0] = 0 and for k ≥ 0, w[3k] = 0, w[3k+1] = 1, w[3k+2] = 2
 * according to the substitution 0→01, 1→02, 2→0.
 * Then a(n) = w[n+1] – w[n].
 * The values lie in {‑2,‑1,0,1,2}.
 */
#include <stdlib.h>
#include <stdint.h>

/* static cache holding the tribonacci word up to the current length */
static unsigned char *trib_word = NULL; /* symbols 0,1,2 */
static size_t         trib_len  = 0;    /* number of valid symbols */
static size_t         trib_cap  = 0;    /* allocated capacity */

/* Extend trib_word until it contains at least index n+1 (i.e. length ≥ n+2). */
static void build_trib(idx_t n)
{
    /* initialisation on first call */
    if (trib_word == NULL)
    {
        trib_cap = 16;
        trib_word = (unsigned char *)malloc(trib_cap);
        trib_word[0] = 0;     /* start word */
        trib_len  = 1;
    }

    /* target length */
    size_t target = (size_t)n + 2;
    while (trib_len < target)
    {
        /* next length is at most twice the current length */
        size_t new_cap = trib_len * 2 + 16;
        unsigned char *new_word = (unsigned char *)malloc(new_cap);
        size_t pos = 0;

        for (size_t i = 0; i < trib_len; ++i)
        {
            unsigned char c = trib_word[i];
            if (c == 0)          /* 0 → 01 */
            {
                new_word[pos++] = 0;
                new_word[pos++] = 1;
            }
            else if (c == 1)     /* 1 → 02 */
            {
                new_word[pos++] = 0;
                new_word[pos++] = 2;
            }
            else                 /* 2 → 0 */
            {
                new_word[pos++] = 0;
            }
        }

        free(trib_word);
        trib_word = new_word;
        trib_cap  = new_cap;
        trib_len  = pos;
    }
}

/* required function */
seq_t a(idx_t n)
{
    build_trib(n);
    return (seq_t)trib_word[(size_t)n + 1] - (seq_t)trib_word[(size_t)n];
}
