/*
 * A281543  Number of partitions n = x + y with y >= x > 0 such that x^2 + y^2
 *          or (x^2 + y^2)/2 is prime.
 * https://oeis.org/A281543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 947, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 9355 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This code implements OEIS A281543.
   a(n) is the number of partitions n = x + y with 1 ≤ x ≤ y such that
   x^2 + y^2 is prime, or (x^2 + y^2)/2 is prime. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
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

seq_t a(idx_t n) {
    seq_t count = 0;
    idx_t limit = n / 2;                /* maximum x */
    seq_t n_sq = (seq_t)n * (seq_t)n;   /* n^2 used for fast y^2 computation */

    for (idx_t x = 1; x <= limit; ++x) {
        seq_t xsq = (seq_t)x * (seq_t)x;
        idx_t y = n - x;
        seq_t ysq = (seq_t)y * (seq_t)y;
        seq_t s = xsq + ysq;          /* x^2 + y^2 */

        if (isPrimeGeneric(s)) {
            ++count;
            continue;
        }
        if ((s & 1) == 0) {           /* even, so test s/2 */
            seq_t half = s >> 1;
            if (isPrimeGeneric(half))
                ++count;
        }
    }
    return count;
}
