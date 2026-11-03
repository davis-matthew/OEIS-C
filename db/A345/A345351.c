/*
 * A345351  Triangular numbers such that the sum of cubes of their digits is
 *          prime.
 * https://oeis.org/A345351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) = the n-th triangular number T such that the sum of the *
 *            cubes of the decimal digits of T is a prime. The sequence is
 *            indexed * with an offset of 1 (a(1) is the first such triangular
 *            number). The * implementation searches successive triangular
 *            numbers, tests the digit * cube sums with isPri...
 * Range:     u8 none, u16 n <= 31, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Validated: partial, pass, 38 terms through n = 38 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>
#include <math.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

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

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}
#include <stddef.h>
#include <stdint.h>

/* static cache for found triangular numbers */
static seq_t *cache = NULL;     /* pointer to array of seq_t */
static size_t cache_len = 0;    /* number of elements stored   */
static size_t cache_cap = 0;    /* current allocated capacity  */
static idx_t  last_k   = 0;     /* last triangular index processed */

/* ensure the cache has room for at least one more element */
static void ensure_cache_capacity(void)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *new_ptr = seq_realloc(cache, cache_cap, new_cap);
        cache = new_ptr;
        cache_cap = new_cap;
    }
}

/* compute a(n) */
seq_t a(idx_t n)
{
    /* n is 1‑based, ensure cache contains at least n elements */
    while (cache_len < (size_t)n) {
        /* next triangular index */
        last_k++;

        /* triangular number T_k = k*(k+1)/2, computed in seq_t */
        seq_t T = (seq_t)last_k * ((seq_t)last_k + (seq_t)1) / (seq_t)2;

        /* sum of cubes of decimal digits of T */
        seq_t sum = (seq_t)0;
        seq_t temp = T;
        while (temp > (seq_t)0) {
            uint8_t d = (uint8_t)(temp % (seq_t)10);
            seq_t d3 = (seq_t)d * (seq_t)d * (seq_t)d;
            sum += d3;
            temp /= (seq_t)10;
        }

        /* if the sum is prime, store the triangular number */
        if (isPrimeGeneric(sum))
            ensure_cache_capacity(),
            cache[cache_len++] = T;
    }

    return cache[(size_t)n - (size_t)1];
}
