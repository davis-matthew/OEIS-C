/*
 * A260072  Primes p such that (p-1)^2+1 divides 2^(p-1)-1.
 * https://oeis.org/A260072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   mod_pow, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) of OEIS A260072:
   Primes p such that (p-1)^2+1 divides 2^(p-1)-1.
   The function generates terms on demand, memoising the results. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/next_prime.h"
#include "../utils/mod_pow.h"

static seq_t* cache = NULL;          /* array of found primes */
static idx_t  cache_count = 0;       /* number of terms cached */
static size_t cache_cap   = 0;       /* current capacity of cache */
static uint64_t last_prime = 2;      /* the last prime tested */

/* Ensure the cache can hold at least need additional elements */
static void ensure_capacity(idx_t need)
{
    if (cache_count + need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (cache_count + need > new_cap) new_cap *= 2;
    seq_t* new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) { /* allocation failed – abort quietly */
        abort();
    }
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n<1, return 0 */
    if (n <= cache_count) return cache[n-1];

    /* generate terms until we have n terms */
    while (cache_count < n)
    {
        /* find next prime */
        last_prime = next_prime(last_prime + 1);
        /* (p-1)^2 + 1 */
        seq_t p_minus_1 = (seq_t)(last_prime - 1);
        seq_t val = p_minus_1 * p_minus_1 + 1;

        /* check divisibility: 2^(p-1) ≡ 1 (mod val) */
        seq_t pow_res = mod_pow((seq_t)2, (uint64_t)(last_prime - 1), val);
        if (pow_res == (seq_t)1)
        {
            ensure_capacity(1);
            cache[cache_count++] = (seq_t)last_prime;
        }
    }
    return cache[n-1];
}
