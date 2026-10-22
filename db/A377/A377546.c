/*
 * A377546  Expansion of e.g.f. (1/x) * Series_Reversion( x*(1 - x*exp(x))^2
 *          ).
 * https://oeis.org/A377546
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * n! * Sum_{k=0..n} k^(n-k) * binomial(2*n+k+2, k) /
 *            ((2*n+k+2)*(n-k)!)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/mul_small.h"

/* This implementation computes the term in a way that avoids
   64‑bit overflow by using an unsigned __int128 intermediate
   when necessary.  For builds that use a big‑integer seq_t,
   the conversion from __int128 to seq_t is handled by the
   constructor of the big‑integer class. */
seq_t a(idx_t n)
{
    static seq_t *fact_cache = NULL;
    static size_t fact_cap = 0;
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    /* ensure factorial cache up to n */
    if (n >= fact_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(fact_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        fact_cache = tmp;
        size_t start = fact_cap == 0 ? 1 : fact_cap;
        if (fact_cap == 0) fact_cache[0] = 1;
        for (size_t i = start; i <= (size_t)n; ++i)
            fact_cache[i] = mul_small(fact_cache[i - 1], (unsigned int)i);
        fact_cap = new_cap;
    }

    /* ensure memoisation cache up to n */
    if (n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        for (size_t i = cache_cap; i < new_cap; ++i)
            cache[i] = 0;               /* sentinel for “not computed” */
        cache_cap = new_cap;
    }

    if (cache[n] != 0)
        return cache[n];

    if (n == 0) {
        cache[0] = 1;
        return 1;
    }

    seq_t sum = 0;
    seq_t two_fact_n = mul_small(fact_cache[(size_t)n], 2);

    for (idx_t k = 1; k <= n; ++k) {
        seq_t k_pow = big_pow((seq_t)k, (seq_t)(n - k));
        idx_t choose_n = 2 * n + k + 2;

        /* compute binomial(2*n+k+2, k) multiplicatively */
        seq_t bin_val = 1;
        if (k > 0) {
            for (idx_t i = 1; i <= k; ++i) {
                bin_val = mul_small(bin_val, (unsigned int)(choose_n - k + i));
                bin_val = bin_val / (seq_t)i;
            }
        }

        /* compute term using 128‑bit intermediate to avoid overflow */
        unsigned __int128 tmp = (unsigned __int128)two_fact_n *
                                (unsigned __int128)k_pow;
        tmp *= (unsigned __int128)bin_val;
        tmp /= (unsigned __int128)choose_n;
        tmp /= (unsigned __int128)fact_cache[(size_t)(n - k)];
        seq_t term = (seq_t)tmp;

        sum += term;
    }

    cache[n] = sum;
    return sum;
}
