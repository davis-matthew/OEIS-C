/*
 * A386088  Primes having only {1, 2, 3, 7} as digits.
 * https://oeis.org/A386088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 225, u32 n <= 51, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_capacity, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes primes whose decimal digits belong to {1,2,3,7} */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include "../utils/is_prime_big.h"      /* bool is_prime_big(seq_t) */
#include "../utils/ensure_capacity.h"   /* void ensure_capacity(size_t, size_t *, void **, size_t) */

/* Allowed decimal digits in ascending order. */
static const uint8_t allowed_digits[4] = {1, 2, 3, 7};

/* Cache of primes already found. */
static seq_t *prime_cache = NULL;   /* array of seq_t */
static size_t cache_cap   = 0;       /* allocated capacity */
static size_t cache_len   = 0;       /* number of primes stored */

/* Current index representation of the candidate number.  index 0 = '1', 1 = '2', … */
static uint8_t *idx_arr      = NULL; /* array of indices into allowed_digits */
static size_t   idx_len      = 0;     /* number of digits in the current candidate */
static size_t   idx_cap      = 0;     /* allocated capacity for idx_arr */

/* Initialise static structures on first use. */
static void init_state(void)
{
    if (prime_cache == NULL) {
        /* initial space for 64 primes */
        ensure_capacity(64, &cache_cap, (void **)&prime_cache, sizeof(seq_t));
    }
    if (idx_arr == NULL) {
        /* initial space for 8 digits */
        ensure_capacity(8, &idx_cap, (void **)&idx_arr, sizeof(uint8_t));
        idx_len = 1;
        idx_arr[0] = 0; /* first digit '1' */
    }
}

/* Increment the digit index array, expanding the length when needed. */
static void increment_indices(void)
{
    int64_t pos = (int64_t)idx_len - 1; /* index of the least‑significant digit */

    while (pos >= 0) {
        if (idx_arr[pos] == 3) {  /* digit '7' → carry */
            idx_arr[pos] = 0;
            pos--;
        } else {
            idx_arr[pos]++;       /* simple increment */
            return;
        }
    }

    /* Completely carried over: create a new most‑significant digit (always '1'). */
    idx_len++;
    if (idx_len > idx_cap) {
        ensure_capacity(idx_len, &idx_cap, (void **)&idx_arr, sizeof(uint8_t));
    }
    for (size_t i = 0; i < idx_len; ++i)
        idx_arr[i] = 0;
}

/* Build the integer value represented by the current digit indices. */
static seq_t current_value(void)
{
    seq_t val = 0;
    for (size_t i = 0; i < idx_len; ++i) {
        val = val * 10 + (seq_t)allowed_digits[idx_arr[i]];
    }
    return val;
}

/* Ensure that at least `target` primes have been cached. */
static void ensure_primes_upto(idx_t target)
{
    init_state();
    while (cache_len < target) {
        seq_t val = current_value();

        if (is_prime_big(val)) {
            if (cache_len >= cache_cap) {
                ensure_capacity(cache_len + 64, &cache_cap,
                                (void **)&prime_cache, sizeof(seq_t));
            }
            prime_cache[cache_len++] = val;
        }
        increment_indices();
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n = 0, but safe fallback */
    ensure_primes_upto(n);
    return prime_cache[n - 1];
}
