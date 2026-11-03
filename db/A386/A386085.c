/*
 * A386085  Primes having only {1, 2, 3, 4} as digits.
 * https://oeis.org/A386085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the nth prime whose decimal representation contains *
 *            exclusively the digits 1, 2, 3, and 4, ordered increasingly by
 *            value
 * Range:     u8 n <= 14, u16 n <= 184, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 65 terms through n = 9824 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <limits.h>
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

/* ------------------------------------------------------------ */
static seq_t build_num(uint64_t idx, unsigned int len)
{
    /* convert idx to base‑4 digits, least significant first   */
    uint8_t dig[64];          /* enough for any len we need            */
    uint64_t tmp = idx;
    for (unsigned int i = 0; i < len; ++i) {
        dig[i] = tmp % 4;
        tmp   /= 4;
    }

    /* construct the decimal number from most significant digit */
    seq_t num = 0;
    for (int i = (int)len - 1; i >= 0; --i)
        num = num * 10 + (seq_t)(dig[i] + 1);

    return num;
}

/* ------------------------------------------------------------ */
/* static cache for the primes already found                   */
/* ------------------------------------------------------------ */
static seq_t *prime_cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap   = 0;

static void cache_append(seq_t val)
{
    if (cache_size == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *newp = mem_new(seq_t, new_cap);
        if (cache_cap) {
            mem_copy(newp, prime_cache, cache_size * sizeof(seq_t));
            mem_free(prime_cache);
        }
        prime_cache = newp;
        cache_cap   = new_cap;
    }
    prime_cache[cache_size++] = val;
}

/* ------------------------------------------------------------ */
/* helpers for generating the next number with digits 1‑4      */

/* state kept between calls to a(n) */
static unsigned int cur_len   = 1;      /* current digit length   */
static uint64_t   cur_limit  = 4;     /* 4^cur_len              */
static uint64_t   cur_idx    = 0;      /* next index to test     */

/* ------------------------------------------------------------ */
/* main sequence function                                        */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset is 1; unused slot   */

    /* generate further primes until we have at least n */
    while (cache_size < n) {
        /* move to the next length when all numbers of current length are exhausted */
        if (cur_idx >= cur_limit) {
            ++cur_len;
            cur_limit *= 4;       /* 4^cur_len */
            cur_idx = 0;
        }

        seq_t cand = build_num(cur_idx, cur_len);
        if (isPrimeGeneric(cand))
            cache_append(cand);

        ++cur_idx;
    }

    return prime_cache[n - 1];
}
