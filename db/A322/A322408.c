/*
 * A322408  Compound sequence with a(n) = A319198(A278041(n)), for n >= 0.
 * https://oeis.org/A322408
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3 + Sum_{k=0}^{n‑1} d(k), where d(k) = 4 if t_k∈{0,1},
 *            else 3
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>


static uint8_t *t_word   = NULL;   /* current tribonacci word symbols {0,1,2} */
static size_t word_len   = 0;       /* its length */

/* sequence cache */
static seq_t *a_arr = NULL;
static size_t a_len = 0;            /* number of stored a values */

/* Ensure t_word has at least 'needed' symbols (indices 0 .. needed‑1). */
static void ensure_word(size_t needed)
{
    if (needed == 0)
        return;
    if (word_len >= needed)
        return;

    if (word_len == 0) {
        t_word = (uint8_t *)malloc(sizeof(uint8_t));
        t_word[0] = 0;
        word_len = 1;
    }
    while (word_len < needed) {
        /* compute length of next word according to the morphism 0→01, 1→02, 2→0 */
        size_t new_len = 0;
        for (size_t i = 0; i < word_len; ++i) {
            uint8_t s = t_word[i];
            new_len += (s == 0 || s == 1) ? 2 : 1;
        }

        uint8_t *next = (uint8_t *)malloc(new_len * sizeof(uint8_t));
        size_t pos = 0;
        for (size_t i = 0; i < word_len; ++i) {
            uint8_t s = t_word[i];
            if (s == 0) {
                next[pos++] = 0;
                next[pos++] = 1;
            } else if (s == 1) {
                next[pos++] = 0;
                next[pos++] = 2;
            } else {                /* s == 2 */
                next[pos++] = 0;
            }
        }
        free(t_word);
        t_word   = next;
        word_len = new_len;
    }
}

/* Ensure a_arr has values up to index 'needed'. */
static void ensure_a(size_t needed)
{
    if (a_len > 0 && needed < a_len)
        return;                     /* already have enough */

    size_t new_len = needed + 1;
    if (a_arr == NULL) {
        a_arr = (seq_t *)malloc(new_len * sizeof(seq_t));
        a_len = 0;                  /* will be updated after init */
    } else if (new_len > a_len) {
        a_arr = (seq_t *)realloc(a_arr, new_len * sizeof(seq_t));
    }

    size_t old_len = a_len;          /* number of values already stored */
    if (old_len == 0) {
        /* first initialization */
        a_arr[0] = (seq_t)3;
        old_len = 1;
    }

    ensure_word(needed);

    for (size_t i = old_len; i <= needed; ++i) {
        uint8_t sym = t_word[i - 1];
        seq_t inc = (sym == 2) ? (seq_t)3 : (seq_t)4;
        a_arr[i] = a_arr[i - 1] + inc;
    }

    a_len = needed + 1;
}

seq_t a(idx_t n)
{
    size_t idx = (size_t)n;
    ensure_a(idx);
    return a_arr[idx];
}
