/*
 * A386356  Primes without {6, 9} as digits.
 * https://oeis.org/A386356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 2323, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   forb_digits_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n-th prime that contains no digits 6 or 9.
   The function builds a static cache of qualifying primes so that calls
   with arbitrary indices are fast.  It uses a simple incremental search
   with trial‑division primality testing. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/forb_digits_uint64.h"

/* primality test for 64‑bit integers (trial division) */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;
    uint64_t i = 5, w = 2;
    while (i * i <= n) {
        if (n % i == 0) return false;
        i += w;
        w = 6 - w;    /* alternate 2 and 4 */
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, return 0 */

    /* Static cache – large enough for n up to 10 000. */
    static seq_t cache[20000];
    static size_t cache_sz = 0;
    static uint64_t last_checked = 1;   /* last examined candidate */

    if (cache_sz >= (size_t)n)
        return cache[n - 1];

    uint64_t candidate = last_checked + 1;
    if (candidate <= 2) candidate = 2;

    while (cache_sz < (size_t)n) {
        /* Skip even numbers > 2 */
        if (candidate > 2 && (candidate & 1) == 0) {
            candidate++;
            continue;
        }

        if (is_prime_uint64(candidate) && !forb_digits_uint64(candidate)) {
            cache[cache_sz++] = (seq_t)candidate;
        }

        last_checked = candidate;
        candidate++;
    }

    return cache[n - 1];
}
