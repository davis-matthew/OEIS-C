/*
 * A351132  G.f. A(x) satisfies: A(x) = x + x^2 * A(x/(1 - 5*x)) / (1 - 5*x).
 * https://oeis.org/A351132
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 0
   a(1) = 1
   a(n) = Σ_{k=0..n-2} binomial(n-2,k) * 5^k * a(n-k-2)
   for all n ≥ 2
*/

#include <stdlib.h>

/* Compute a(n) with memoisation */
seq_t a(idx_t n)
{
    /* static caches */
    static seq_t *cache    = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_max  = 0;

    static seq_t *pow5  = NULL;
    static size_t pow5_cap = 0;

    /* --- extend cache array if needed ----------------------------------- */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 4;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1); /* allocation failed */
        /* initialise the new entries (unnecessary for arbitrary‑precision but safe) */
        for (size_t i = cache_cap; i < new_cap; ++i) tmp[i] = 0;
        cache   = tmp;
        cache_cap = new_cap;
    }

    /* --- extend powers of 5 array if needed ------------------------------ */
    idx_t need_pow5 = (n > 1) ? (n - 1) : 0;   /* 5^0 .. 5^(n-1) are required */
    while (pow5_cap <= need_pow5) {
        size_t new_cap = pow5_cap ? pow5_cap << 1 : 4;
        while (new_cap <= (size_t)need_pow5) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(pow5, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        for (size_t i = pow5_cap; i < new_cap; ++i) {
            if (i == 0) tmp[i] = (seq_t)1;     /* 5^0 = 1 */
            else        tmp[i] = tmp[i-1] * (seq_t)5;
        }
        pow5 = tmp;
        pow5_cap = new_cap;
    }

    /* --- initialise first terms if not already done --------------------- */
    if (cache_max == 0) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache_max = 1;
    }

    /* --- compute all values up to n ------------------------------------- */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        if (i < 2) {
            /* values 0 and 1 already set */
            continue;
        }

        idx_t upto = i - 2;
        seq_t sum = 0;

        /* C = binomial(upto,0) = 1 */
        seq_t C = (seq_t)1;
        for (idx_t m = 0; m <= upto; ++m) {
            idx_t pow_idx = upto - m;            /* exponent of 5 */
            seq_t term = C;
            term *= pow5[pow_idx];
            term *= cache[m];
            sum += term;

            /* update binomial coefficient for next m */
            if (m != upto) {
                C = C * (seq_t)(upto - m) / (seq_t)(m + 1);
            }
        }
        cache[i] = sum;
    }
    cache_max = n;

    return cache[n];
}
