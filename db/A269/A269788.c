/*
 * A269788  Primes p such that 2*p + 47 is a square.
 * https://oeis.org/A269788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‐th prime p such that 2*p + 47 is a perfect square
 * Range:     u8 n <= 4, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"   /* provides bool is_prime_big(seq_t n) */

/* Compute the n-th term of A269788 */
seq_t a(idx_t n)
{
    /* sparse static cache: store found primes in increasing order */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;   /* how many primes stored */
    static size_t cache_cap = 0;   /* allocated capacity */
    static idx_t  next_k     = 9;  /* odd k to test, starts at 9 */

    /* offset is 1; defensive handling of n == 0 */
    if (n == 0) return 0;

    /* initialise cache on first use */
    if (cache_len == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;               /* allocation failed → zero sentinel */
    }

    /* generate more terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        /* candidate prime: p = (k^2 - 47)/2   (k odd) */
        seq_t k  = (seq_t)next_k;
        seq_t prod = k * k;
        seq_t p  = (prod - (seq_t)47) / (seq_t)2;

        if (is_prime_big(p)) {                 /* use provided primality test */
            if (cache_len == cache_cap) {        /* grow capacity if needed */
                cache_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) {                    /* reallocation failed */
                    free(cache);
                    cache = NULL;
                    cache_len = cache_cap = 0;
                    return 0;
                }
                cache = tmp;
            }
            cache[cache_len++] = p;
        }

        next_k += 2;      /* next odd integer */
    }

    return cache[n - 1];
}
