/*
 * A269225  Smallest k such that k! > 2^n.
 * https://oeis.org/A269225
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = smallest k such that k! > 2^n
 * Range:     u8 n <= 1675, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>

/* External types supplied by the build system */
seq_t a(idx_t n)
{
    /* cum[k] = log2(k!) for k >= 0, with cum[0] = 0 (0! = 1). */
    static double *cum = NULL;
    static idx_t cum_cap = 0;   /* capacity of cum array (number of entries) */
    static idx_t cum_len = 0;   /* number of valid entries in cum          */

    /* First call: allocate an initial array. */
    if (cum_len == 0) {
        cum_cap = 64;
        cum = (double *)malloc(cum_cap * sizeof(double));
        if (!cum) return (seq_t)0;   /* allocation failure – unlikely in tests */
        cum[0] = 0.0;                /* 0! = 1, log2(1) = 0 */
        cum_len = 1;
    }

    /* k is the highest factorial index already computed. */
    idx_t k = cum_len - 1;

    /* Compute successive factorial logarithms until we exceed n. */
    while (cum[k] <= (double)n) {
        /* Grow capacity if necessary. */
        if (k + 1 >= cum_cap) {
            size_t new_cap = cum_cap * 2;
            double *tmp = (double *)realloc(cum, new_cap * sizeof(double));
            if (!tmp) break;  /* allocation failure – stop; unlikely in tests */
            cum = tmp;
            cum_cap = new_cap;
        }
        ++k;  /* New factorial index to compute. */
        cum[k] = cum[k - 1] + log2((double)k);  /* log2(k!) = log2((k-1)!) + log2(k) */
    }

    /* Preserve the length for future calls. */
    if (k + 1 > cum_len) cum_len = k + 1;

    return (seq_t)k;
}
