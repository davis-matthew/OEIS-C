/*
 * A386107  Primes having only {1, 3, 4, 7} as digits.
 * https://oeis.org/A386107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the nth prime whose decimal digits belong to {1,3,4,7}
 * Range:     u8 n <= 16, u16 n <= 205, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 67 terms through n = 9212 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

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

/* Allowed digits */
static const seq_t allowed_digits[4] = { (seq_t)1, (seq_t)3, (seq_t)4, (seq_t)7 };

/* Return the nth such prime (a(1) = 3) */
seq_t a(idx_t n)
{
    /* Cached primes found so far */
    static seq_t *cache   = NULL;
    static size_t cap     = 0;
    static idx_t  have    = 0;

    /* State for incrementally generating candidates */
    static idx_t   cur_len      = 1;   /* current number of decimal digits */
    static uint64_t idx_in_len  = 0;   /* index within the current length */
    static uint64_t limit_len   = 4;   /* 4^cur_len, upper bound for idx_in_len */

    /* Return cached result if already computed */
    if (n <= have)
        return cache[n - 1];

    /* Ensure the cache has space for at least 'n' primes */
    ensure_cache(n, &cache, &cap);

    /* Generate candidates until we find the required number of primes */
    while (have < n)
    {
        /* Build the decimal number from the base‑4 representation */
        uint64_t tmp = idx_in_len;
        seq_t val = 0;
        seq_t digits[32];
        int i = 0;

        /* Extract base‑4 digits (least significant first) */
        while (i < cur_len)
        {
            digits[i] = allowed_digits[(int)(tmp % 4)];
            tmp       /= 4;
            ++i;
        }

        /* Assemble the decimal number from most significant digit */
        for (i = cur_len - 1; i >= 0; --i)
        {
            val = val * 10 + digits[i];
        }

        /* Test primality */
        if (isPrimeGeneric(val))
        {
            cache[have++] = val;   /* Store the prime found */
        }

        /* Advance to the next candidate */
        ++idx_in_len;
        if (idx_in_len == limit_len)           /* finished this length */
        {
            idx_in_len = 0;
            ++cur_len;
            limit_len *= 4;                    /* 4^cur_len for the new length */
        }
    }

    return cache[n - 1];
}
