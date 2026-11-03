/*
 * A386123  Primes having only {1, 4, 6, 7} as digits.
 * https://oeis.org/A386123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 128, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 63 terms through n = 9022 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th prime whose decimal digits belong to {1,4,6,7}.
   The sequence is obtained by generating all numbers with those digits in
   increasing numeric order and collecting the primes.  The function is
   memoised so that repeated calls with arbitrary n are efficient. */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
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
static size_t cache_len  = 0;
static size_t cache_cap  = 0;

/* 4 allowed decimal digits, in ascending order. */
static const uint8_t DIGITS[4] = {1, 4, 6, 7};

/* Current candidate represented as an array of indices into DIGITS
   (base‑4 counter).  idx_arr[i] = 0..3.  idx_arr[0] is the most
   significant digit.  digits_len is its logical length. */
static int  idx_arr[64];
static size_t digits_len = 0;

/* Increment the candidate to the next number whose digits are from
   DIGITS.  This is a standard carry in base‑4 with digits mapping. */
static void increment_digits(void)
{
    size_t i = digits_len;
    while (i-- > 0) {
        idx_arr[i] = (idx_arr[i] + 1) & 3;   /* add 1 modulo 4 */
        if (idx_arr[i] != 0)                /* no carry */
            return;
    }
    /* All digits wrapped to 0; prepend a new most significant digit. */
    for (size_t j = digits_len; j > 0; --j)
        idx_arr[j] = idx_arr[j - 1];
    idx_arr[0] = 0;
    ++digits_len;
}

/* Initialise static data structures. */
static void init_sequence(void)
{
    digits_len = 1;           /* start at the one‑digit number '1' */
    idx_arr[0] = 0;           /* DIGITS[0] == 1 */
    cache_cap  = 16;
    prime_cache = mem_new(seq_t, cache_cap);
}

/* Return the n-th prime with only {1,4,6,7} as digits.  n >= 1. */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        init_sequence();
        initialized = true;
    }
    if (n == 0) return 0;           /* guard against offset error */

    while (cache_len < (size_t)n) {
        /* Build the current candidate value from the idx_arr digits. */
        seq_t value = 0;
        for (size_t i = 0; i < digits_len; ++i)
            value = value * 10 + DIGITS[idx_arr[i]];

        /* Check primality and record if prime. */
        if (isPrimeGeneric(value)) {
            if (cache_len >= cache_cap) {
                cache_cap *= 2;
                prime_cache = mem_resize(seq_t, prime_cache, cache_cap);
            }
            prime_cache[cache_len++] = value;
        }

        /* Advance to the next candidate. */
        increment_digits();
    }
    return prime_cache[(size_t)n - 1];
}
