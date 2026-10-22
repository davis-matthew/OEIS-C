/*
 * A320670  G.f.: 1 / [ Sum_{n>=0} (-1)^n * (2*n+1)*(9*n+1) * x^(n*(n+1)/2) ].
 * https://oeis.org/A320670
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(0)=1, and for n>0
 * a(n) = Σ_{k: k(k+1)/2 ≤ n} (-1)^(k+1) * (2k+1)(9k+1) * a(n - k(k+1)/2)  */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;      /* cache of computed terms          */
    static size_t cache_cap = 0;   /* allocated capacity                */
    static size_t cache_len = 0;   /* number of elements already stored */

    /*--- initial allocation -------------------------------------------*/
    if (!seq) {
        cache_cap = 16;                           /* arbitrary start size */
        seq = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        seq[0] = (seq_t)1;                        /* a(0) = 1 */
        cache_len = 1;
    }

    /*--- grow the cache if the requested index is beyond the limit --*/
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < n + 1)
            new_cap <<= 1;                        /* double until enough */
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /*--- compute missing terms up to and including n ---------------*/
    for (idx_t m = cache_len; m <= n; ++m) {
        seq_t sum = 0;
        for (idx_t k = 1; ; ++k) {
            idx_t t = k * (k + 1ULL) / 2ULL;      /* triangular number */
            if (t > m) break;

            seq_t coeff = (seq_t)(2ULL * k + 1ULL) * (seq_t)(9ULL * k + 1ULL);
            seq_t term  = coeff * seq[m - t];

            if (k & 1)      /* k odd → add   */
                sum += term;
            else            /* k even → subtract */
                sum -= term;
        }
        seq[m] = sum;
    }
    cache_len = n + 1;
    return seq[n];
}
