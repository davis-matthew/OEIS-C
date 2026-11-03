/*
 * A327348  The 66 prime dates in each non-leap year of the form concatenate
 *          (month,day) without leading zeros for days.
 * https://oeis.org/A327348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, fini, full, changed
 * Formula:   a(n) is the n-th prime obtained by concatenating the month and
 *            the day (without leading zeros) of a non‑leap year. The sequence
 *            contains the 66 such primes
 * Range:     u8 n <= 14, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Validated: full, pass, 66 terms through n = 66 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/num_digits_idx.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
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

seq_t a(idx_t n)
{
    /*  Days in each month of a non‑leap year (index 1 .. 12). */
    static const unsigned month_days[13] = {
        0, 31, 28, 31, 30, 31, 30,
        31, 31, 30, 31, 30, 31
    };

    /*  Cache of the 66 prime dates.  It is filled on the first call. */
    static seq_t  cache[66];
    static int    depth = 0;          /* number of primes stored */
    static bool   initialized = false;

    if (!initialized) {
        int idx = 0;
        for (unsigned month = 1; month <= 12; ++month) {
            for (unsigned day = 1; day <= month_days[month]; ++day) {
                int dlen  = num_digits_idx((idx_t)day);                 /* number of digits in day */
                seq_t num = (seq_t)month * pow10((idx_t)dlen) + (seq_t)day;
                if (isPrimeGeneric(num)) {
                    if (idx < 66) {
                        cache[idx++] = num;
                    }
                }
            }
        }
        depth = idx;      /* should be 66 for a non‑leap year */
        initialized = true;
    }

    if (n == 0 || n > (idx_t)depth) {
        return (seq_t)0;
    }
    return cache[(size_t)(n - 1)];
}
