/*
 * A287407  Start with 0 and repeatedly substitute 0->012, 1->210, 2->201.
 * https://oeis.org/A287407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: The infinite word is the fixed point of the morphism
 *   0 → 012
 *   1 → 210
 *   2 → 201
 * starting from the single symbol 0.  a(n) (1‑based) is the digit
 * at position n‑1 of that word.  The function expands the word
 * until it is long enough for the requested index.  Complexity is
 * O(n) per distinct n and stays well under 2 s for n up to about 10 000.
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state keeps the generated prefix between calls */
    static seq_t *w = NULL;        /* buffer holding the current word   */
    static size_t len = 0;         /* number of symbols currently stored */

    /* offset 1 : undefined for n<1, return 0 harmlessly */
    if (n < 1)
        return (seq_t)0;

    /* initialise the sequence with the seed "0" if not yet done */
    if (len == 0) {
        w = (seq_t*)malloc(sizeof(seq_t));
        if (!w)       /* allocation failed */
            return (seq_t)0;
        w[0] = (seq_t)0;
        len = 1;
    }

    /* grow until we have at least n symbols */
    while (len < (size_t)n) {
        size_t old_len = len;
        size_t new_len = old_len * 3;

        /* realloc may change the address; cast to seq_t* for C++ */
        seq_t *tmp = (seq_t*)realloc(w, new_len * sizeof(seq_t));
        if (!tmp) {   /* allocation failed */
            return (seq_t)0;
        }
        w   = tmp;
        len = new_len;

        /* expand in reverse to avoid overwriting data before it is read */
        size_t i, j;
        for (i = old_len; i-- > 0;) {
            seq_t d = w[i];
            j = i * 3 + 2;
            /* write the three mapped digits in the correct order */
            if (d == 0) {          /* 0 → 0 1 2 */
                w[j-2] = (seq_t)0;
                w[j-1] = (seq_t)1;
                w[j]   = (seq_t)2;
            } else if (d == 1) {   /* 1 → 2 1 0 */
                w[j-2] = (seq_t)2;
                w[j-1] = (seq_t)1;
                w[j]   = (seq_t)0;
            } else {               /* 2 → 2 0 1 */
                w[j-2] = (seq_t)2;
                w[j-1] = (seq_t)0;
                w[j]   = (seq_t)1;
            }
        }
    }

    /* return the (n-1)-th element (0‑based index) */
    return w[n - 1];
}
