/*
 * A385990  a(0) = 1; a(n) = Sum_{k=0..n-1} (2^k + 3^k) * binomial(n-1,k) *
 *          a(k) * a(n-1-k).
 * https://oeis.org/A385990
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = Sum_{k=0..n-1} (2^k + 3^k) * binomial(n-1,k) *
 *            a(k) * a(n-1-k) (n ≥ 1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* uses the generic seq_t/idx_t types supplied by the build */
seq_t a(idx_t n)
{
    /* static buffers that store computed values and powers */
    static seq_t *table = NULL;      /* a(i) */
    static seq_t *pow2  = NULL;      /* 2^i */
    static seq_t *pow3  = NULL;      /* 3^i */
    static size_t cap   = 0;         /* allocated capacity */
    static idx_t  len   = 0;         /* number of computed elements (len == highest index + 1) */

    /* allocate or grow buffers if needed */
    if (n + 1 > cap) {
        size_t newcap = (cap == 0 ? 16 : cap * 2);
        while (newcap < (size_t)(n + 1))
            newcap *= 2;
        seq_t *tmp = (seq_t *)realloc(table, newcap * sizeof(seq_t));
        if (!tmp) exit(1);
        table = tmp;
        tmp = (seq_t *)realloc(pow2, newcap * sizeof(seq_t));
        if (!tmp) exit(1);
        pow2 = tmp;
        tmp = (seq_t *)realloc(pow3, newcap * sizeof(seq_t));
        if (!tmp) exit(1);
        pow3 = tmp;
        cap = newcap;
    }

    /* initialise first element if needed */
    if (len == 0) {
        table[0] = 1;
        pow2[0]  = 1;
        pow3[0]  = 1;
        len = 1;
    }

    /* compute missing values up to n */
    for (idx_t i = len; i <= n; ++i) {
        /* compute powers for the next index (needed on next iteration) */
        pow2[i] = pow2[i - 1] * (seq_t)2;
        pow3[i] = pow3[i - 1] * (seq_t)3;

        seq_t sum   = 0;
        seq_t binom = 1;            /* binomial(n-1,k) for current k */
        for (idx_t k = 0; k < i; ++k) {
            /* term = a(k) * a(i-1-k) * binom * (2^k + 3^k) */
            seq_t term = table[k];
            term = term * table[i - 1 - k];
            term = term * binom;
            seq_t powsum = pow2[k] + pow3[k];
            term = term * powsum;
            sum += term;

            /* prepare binom for next k */
            if (k + 1 < i) {
                binom = binom * (seq_t)(i - 1 - k);
                binom = binom / (seq_t)(k + 1);
            }
        }
        table[i] = sum;
    }

    len = (idx_t)(n + 1);
    return table[n];
}
