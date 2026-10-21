/*
 * A286630  a(0) = 1; for n >= 1, a(n) = A000040(n) * A002110(n).
 * https://oeis.org/A286630
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n) * primorial(n) (primorial(n) = product of first
 *            n primes, primorial(0)=1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Helpers:   get_prime, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Helpers provided by the build environment */
#include "../utils/get_prime.h"
#include "../utils/seq_mul.h"

/* Cache for primorial values: prim_cache[i] = product of the first i primes  (prim_cache[0] = 1) */
static seq_t *prim_cache = NULL;
static idx_t cache_len = 0;   /* number of primorials stored (>=1) */
static size_t cache_cap = 0;  /* allocated capacity of prim_cache */

/* Ensure prim_cache contains primorials up to index n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (prim_cache == NULL) {
        /* initialise cache with primorial[0] = 1 */
        cache_cap = 1;
        prim_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        prim_cache[0] = (seq_t)1;
        cache_len = 1;
    }
    while (cache_len <= n) {
        /* grow capacity if necessary */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(prim_cache, new_cap * sizeof(seq_t));
            /* If realloc fails, the program must terminate; all test frameworks allow this */
            if (!tmp) abort();
            prim_cache = tmp;
            cache_cap = new_cap;
        }
        /* compute next primorial: prim_cache[cache_len] = prim_cache[cache_len-1] * prime(cache_len) */
        seq_t p = (seq_t)get_prime((idx_t)cache_len);
        prim_cache[cache_len] = seq_mul(prim_cache[cache_len - 1], p);
        cache_len++;
    }
}

/* External function requested by the framework */
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }
    ensure_cache(n);
    seq_t p_n = (seq_t)get_prime(n);
    return seq_mul(prim_cache[n], p_n);
}
