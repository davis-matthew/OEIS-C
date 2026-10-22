/*
 * A345103  a(n) = 1 + 4 * Sum_{k=0..n-1} binomial(n,k) * a(k) * a(n-k-1).
 * https://oeis.org/A345103
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + 4 * ∑_{k=0}^{n-1} binomial(n,k) * a(k) * a(n-k-1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   binomial, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence with a(0) = 1 */

#include <stddef.h>
#include "../utils/binomial.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* stores computed a(k) values */
    static size_t cap   = 0;         /* allocated capacity */
    static size_t sz    = 0;         /* number of values actually computed (next index to compute) */

    /* Grow cache if necessary */
    if ((size_t)n + 1 > cap) {
        size_t newcap = cap ? cap * 2 : 1;
        while (newcap <= (size_t)n) newcap *= 2;
        cache = seq_realloc(cache, cap, newcap);
        cap = newcap;
    }

    /* Compute missing values up to n */
    for (idx_t i = sz; i <= n; ++i) {
        if (i == 0) {
            cache[0] = 1;
            continue;
        }

        seq_t sum = 0;
        for (idx_t k = 0; k < i; ++k) {
            seq_t term = binomial(i, k);
            term *= cache[k];
            term *= cache[i - 1 - k];
            sum += term;
        }

        cache[i] = 1 + 4 * sum;   /* 4 * sum and then 1 + ... */
    }

    sz = n + 1;   /* next index to compute will be n+1 */
    return cache[n];
}
