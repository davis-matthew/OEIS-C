/*
 * A281145  Number of same-trees of weight n.
 * https://oeis.org/A281145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Sum_{d|n, d<n} a(d)^(n/d)
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 47, u64 n <= 95, u128 n <= 167,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/big_pow.h"

static seq_t *cache = NULL;
static idx_t max_computed = 0;

/* Ensure cache entries up to n are computed */
static void ensure_cache(idx_t n)
{
    if (n <= max_computed) return;

    size_t new_cap = (size_t)n + 1;           /* indices 0..n */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);                       /* out of memory */
    cache = tmp;

    /* Compute values from (max_computed+1) up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i)
    {
        if (i == 1) { cache[i] = 1; continue; }

        seq_t sum = 0;
        for (idx_t d = 1; d <= i/d; ++d)     /* avoid overflow in d*d */
        {
            if (i % d == 0)
            {
                if (d < i) {
                    seq_t pw = big_pow(cache[d], (seq_t)(i / d));
                    sum += pw;
                }
                idx_t other = i / d;
                if (other != d && other < i) {
                    seq_t pw = big_pow(cache[other], (seq_t)(i / other));
                    sum += pw;
                }
            }
        }
        cache[i] = sum + 1;
    }
    max_computed = n;
}

/* Public function implementing the sequence */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1 */
    ensure_cache(n);
    return cache[n];
}
