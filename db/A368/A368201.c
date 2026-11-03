/*
 * A368201  Least k such that 9*n*k+1 is a prime.
 * https://oeis.org/A368201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest k ≥ 1 such that 9 · n · k + 1 is prime
 * Range:     u8 n <= 12, u16 n <= 3339, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 101 terms through n = 4923 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
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

seq_t a(idx_t n)
{
    /* If n < 1, return 0 – the offset starts at 1, but this case is safe. */
    if (n == 0)
        return (seq_t)0;

    idx_t k = 1;
    for (;; ++k) {
        /* Compute 9*n*k + 1 using seq_t to avoid overflow of idx_t. */
        seq_t val = (seq_t)9 * (seq_t)n * (seq_t)k + (seq_t)1;
        if (isPrimeGeneric(val))
            return (seq_t)k;
    }
}
