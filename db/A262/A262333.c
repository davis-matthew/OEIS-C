/*
 * A262333  Number of (n+3) X (1+3) 0..1 arrays with each row and column
 *          divisible by 9, read as a binary number with top and left being
 *          the most significant bits.
 * https://oeis.org/A262333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) - a(n-3) + 3*a(n-4) - 2*a(n-5) with
 *            initial values a(1)=2, a(2)=4, a(3)=8, a(4)=15, a(5)=29
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence A262333, defined for n >= 1. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of computed terms. 1‑based indexing. */
    static seq_t *c = NULL;
    static idx_t len = 0;
    static idx_t cap = 0;

    if (n < 1) return 0;

    /* Initial allocation and seed the first five terms. */
    if (!c) {
        cap = 128;                                 /* arbitrary starting size */
        c = (seq_t *)malloc((cap + 1) * sizeof(seq_t));
        if (!c) return 0;                          /* allocation failure */
        c[1] = (seq_t)2;
        c[2] = (seq_t)4;
        c[3] = (seq_t)8;
        c[4] = (seq_t)15;
        c[5] = (seq_t)29;
        len = 5;
    }

    /* Grow the cache if necessary. */
    while (len < n) {
        if (cap <= len) {
            cap = cap * 2 + 16;
            c = (seq_t *)realloc(c, (cap + 1) * sizeof(seq_t));
            if (!c) return 0;                      /* allocation failure */
        }
        idx_t k = len + 1;
        seq_t val;
        if (k <= 5) {                              /* safety, although unused */
            switch (k) {
                case 1: val = (seq_t)2; break;
                case 2: val = (seq_t)4; break;
                case 3: val = (seq_t)8; break;
                case 4: val = (seq_t)15; break;
                case 5: val = (seq_t)29; break;
                default: val = 0; break;
            }
        } else {                                   /* recurrence */
            const seq_t two  = (seq_t)2;
            const seq_t three= (seq_t)3;
            val = c[k-1] * three;                 /* 3*a(k-1) */
            val += c[k-4] * three;                /* +3*a(k-4) */
            val -= c[k-2] * two;                  /* -2*a(k-2) */
            val -= c[k-3];                        /* -a(k-3) */
            val -= c[k-5] * two;                  /* -2*a(k-5) */
        }
        c[k] = val;
        len = k;
    }
    return c[n];
}
