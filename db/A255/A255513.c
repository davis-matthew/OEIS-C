/*
 * A255513  Numbers k such that 24*k+13, 72*k+37 and 192*k+97 are all prime.
 * https://oeis.org/A255513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 13, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 65 terms through n = 9444 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements OEIS A255513:
   a(n) is the n‑th non‑negative integer k such that
   24*k+13, 72*k+37, and 192*k+97 are all prime.  The function
   searches sequentially and memoizes results. */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

#ifdef __cplusplus
    #include <vector>
    /* C++ implementation using std::vector */
    static std::vector<seq_t> cache;
    static seq_t last_checked = 0;
    static bool initialized = false;
    
    static void init_cache_vector(void)
    {
        if (initialized) return;
        cache.push_back((seq_t)0);   /* a(1) = 0 */
        last_checked = 0;
        initialized = true;
    }
    
    seq_t a(idx_t n)
    {
        if (n < 1) return 0;
        init_cache_vector();
        /* cache[0] holds a(1).  Fill until we have at least n entries. */
        while ( (idx_t)cache.size() < n )
        {
            seq_t k = last_checked + 1;
            seq_t v1 = 24 * k + 13;
            seq_t v2 = 72 * k + 37;
            seq_t v3 = 192 * k + 97;
            if ( isPrimeGeneric(v1) && isPrimeGeneric(v2) && isPrimeGeneric(v3) )
                cache.push_back(k);
            last_checked = k;
        }
        return cache[(size_t)(n - 1)];
    }
#else
    /* C implementation using a plain array */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t cache_cap   = 0;
    static seq_t last_checked = 0;
    
    static void ensure_cache_capacity(idx_t needed)
    {
        if (needed <= cache_cap) return;
        idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (cache) {
            memcpy(new_cache, cache, cache_size * sizeof(seq_t));
            free(cache);
        }
        cache = new_cache;
        cache_cap = new_cap;
    }
    
    seq_t a(idx_t n)
    {
        if (n < 1) return 0;
        /* One time initialization of the cache with a(1)=0 */
        if (cache == NULL) {
            cache_size = 1;
            cache_cap   = 16;
            cache       = (seq_t *)malloc(cache_cap * sizeof(seq_t));
            cache[0] = 0;        /* a(1) = 0 */
            last_checked = 0;
        }
        while (cache_size < n) {
            seq_t k = last_checked + 1;
            seq_t v1 = 24 * k + 13;
            seq_t v2 = 72 * k + 37;
            seq_t v3 = 192 * k + 97;
            if ( isPrimeGeneric(v1) && isPrimeGeneric(v2) && isPrimeGeneric(v3) ) {
                ensure_cache_capacity(cache_size + 1);
                cache[cache_size++] = k;
            }
            last_checked = k;
        }
        return cache[n - 1];
    }
#endif
