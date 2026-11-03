/*
 * A347159  Sum of cubes of distinct prime divisors of n that are <= sqrt(n).
 * https://oeis.org/A347159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{p|n, p ≤ √n} p³, summing over distinct prime divisors
 *            of n
 * Range:     u8 n <= 48, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Validated: partial, pass, 75 terms through n = 75 (2026-11-01)
 * Helpers:   factor_u64, isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include the helper utilities used below */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/isqrt.h"

seq_t a(idx_t n)
{
    /* Compute the floor of the square‑root of n */
    idx_t sqrt_n = isqrt((uint64_t)n);

    /* Factor n into distinct primes (exponents are ignored) */
    uint64_t primes[64];
    uint64_t exponents[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exponents, &cnt);

    /* Accumulate the sum of cubes of qualifying primes */
    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        if (p <= sqrt_n) {
            seq_t pseq = (seq_t)p;
            sum += (pseq * pseq * pseq);
        }
    }
    return sum;
}
