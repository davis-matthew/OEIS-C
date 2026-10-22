/*
 * A360818  Expansion of Sum_{k>=0} ( (k*x)^2 / (1 - k*x) )^k.
 * https://oeis.org/A360818
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = Sum_{k=0..⌊n/2⌋} k^n * binomial(n-k-1, n-2*k) * (with
 *            the convention that a(0)=1)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/binomial.h"   /* seq_t binomial(idx_t n, idx_t k) */
#include "../utils/big_pow.h"    /* seq_t big_pow(seq_t base, seq_t exp) */

static seq_t *cache = NULL;     /* cached values of a(n) */
static idx_t cache_size = 0;    /* number of cached entries (highest index+1) */

/* Compute the OEIS sequence A360818 */
seq_t a(idx_t n) {
    /* Return cached value if available */
    if (cache && n < cache_size) {
        return cache[n];
    }

    /* Compute a(n) from the definition */
    seq_t result = (seq_t)0;

    if (n == 0) {
        result = (seq_t)1;
    } else {
        idx_t kmax = n >> 1; /* floor(n/2) */
        for (idx_t k = 1; k <= kmax; ++k) {
            seq_t pk   = big_pow((seq_t)k, (seq_t)n);                    /* k^n */
            seq_t comb = binomial(n - k - 1, n - 2 * k);                /* binomial(n-k-1, n-2k) */
            result += pk * comb;                                         /* k^n * binomial(...) */
        }
    }

    /* Extend the cache to hold up to index n */
    if (cache == NULL || n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_size * sizeof(seq_t));
        if (tmp == NULL) {
            /* Allocation failed: fall back to returning result without caching */
            return result;
        }
        cache = tmp;
        cache_size = new_size;
    }

    /* Store the result in the cache */
    cache[n] = result;
    return result;
}
