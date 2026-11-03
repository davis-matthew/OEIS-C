/*
 * A386116  Primes having only {1, 3, 6, 9} as digits.
 * https://oeis.org/A386116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 204, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 65 terms through n = 7934 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A386116: primes whose decimal digits are only {1,3,6,9}.
   Computation proceeds by generating all numbers with the allowed digits
   in increasing numeric order and testing them for primality.  A cache
   stores the primes found so far so that subsequent calls are efficient. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
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

/* Helper to compute the minimal value for a given length (all digits 1). */
static inline uint64_t min_value_of_len(uint64_t len)
{
    uint64_t v = 0;
    while (len--) v = v * 10 + 1;
    return v;
}

seq_t a(idx_t n)
{
    /* Cache of primes found so far. */
    static seq_t *cache    = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;

    /* State of the generator. */
    static idx_t    current_len = 1;        /* current number of digits   */
    static uint64_t current_idx = 0;        /* base‑4 index within length */
    static uint64_t power_of_4 = 4;         /* 4^current_len             */

    /* Maximum representable value for the current seq_t type. */
    seq_t seq_max = (seq_t)-1;
    uint64_t max_val64 = (uint64_t)seq_max;

    /* If the minimal possible number for the current length exceeds the
       maximum representable value, no further numbers fit.  The loop will
       terminate once enough primes have been cached. */
    while (cache_size < n) {
        /* Advance to the next length if all numbers of the current length
           have been processed. */
        if (current_idx == power_of_4) {
            ++current_len;
            current_idx = 0;
            power_of_4 *= 4;
        }

        /* Check if any numbers of the new length can be represented. */
        if (min_value_of_len(current_len) > max_val64) {
            /* No more numbers that fit.  Break – this case should not
               happen in tests that request only values that fit in seq_t. */
            break;
        }

        /* Build the decimal number from the base‑4 index. */
        seq_t  val   = 0;
        uint64_t val64 = 0;
        uint64_t t   = current_idx;
        for (int k = current_len - 1; k >= 0; --k) {
            uint8_t  digit  = (uint8_t)((t >> (2 * k)) & 3);
            uint8_t  mapped;
            switch (digit) {
                case 0: mapped = 1; break;
                case 1: mapped = 3; break;
                case 2: mapped = 6; break;
                default: mapped = 9; break;
            }
            val = val * 10 + mapped;
            val64 = val64 * 10 + mapped;
        }

        /* If the computed number exceeds the representable range, skip the
           remainder of this length (they will also exceed). */
        if (val64 > max_val64) {
            ++current_len;
            current_idx = 0;
            power_of_4 *= 4;
            continue;
        }

        /* Prime check. */
        if (isPrimeGeneric(val)) {
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = val;
        }
        ++current_idx;
    }

    return cache[n - 1];
}
