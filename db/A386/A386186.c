/*
 * A386186  Primes having only {3, 6, 8, 9} as digits.
 * https://oeis.org/A386186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n‑th prime whose decimal digits are all in {3,6,8,9}
 * Range:     u8 n <= 3, u16 n <= 71, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 62 terms through n = 9606 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
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

static seq_t *prime_cache = NULL;
static size_t cache_size = 0;    /* number of primes stored */
static size_t cache_cap  = 0;    /* allocated capacity */
static size_t current_len = 0;   /* current maximum length of generated numbers */
static const unsigned char digit_map[4] = {3, 6, 8, 9};

/* Append a prime to the cache, resizing if necessary */
static void cache_append(seq_t val)
{
    if (cache_size == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap << 1;
        seq_t *tmp = mem_resize(seq_t, prime_cache, new_cap);
        if (!tmp) abort();          /* out of memory */
        prime_cache = tmp;
        cache_cap = new_cap;
    }
    prime_cache[cache_size++] = val;
}

/* Recursively generate all numbers of a given length composed of {3,6,8,9} */
static void generate_len(size_t len, size_t pos, seq_t cur)
{
    if (pos == len) {
        if (isPrimeGeneric(cur))
            cache_append(cur);
        return;
    }
    for (size_t i = 0; i < 4; ++i) {
        seq_t next = cur * (seq_t)10 + (seq_t)digit_map[i];
        generate_len(len, pos + 1, next);
    }
}

/* Ensure that the cache contains at least n primes */
static void ensure_cache(idx_t n)
{
    while (cache_size < n) {
        ++current_len;
        generate_len(current_len, 0, (seq_t)0);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset not used; safety */
    ensure_cache(n);
    return prime_cache[n - 1];
}
