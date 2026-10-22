/*
 * A333909  Numbers k such that phi(k) is the sum of 2 squares, where phi is
 *          the Euler totient function (A000010).
 * https://oeis.org/A333909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 121, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm:  a(n) = the n‑th integer k such that phi(k) is a sum of two squares.
   The function searches integers k in ascending order, computes phi(k),
   tests whether phi(k) can be written as a^2+b^2 by the classical
   characterization (all primes ≡3 (mod 4) appear with even exponent),
   and records the k's until the desired index is reached. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* Compute Euler's totient of m using its prime factorisation. */
static uint64_t phi_uint64(uint64_t m)
{
    if (m == 0)
        return 0;
    if (m == 1)
        return 1;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64(m, primes, exps, &cnt);

    __uint128_t phi = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        __uint128_t term = (uint64_t)(p - 1);
        __uint128_t p_pow = 1;
        for (uint64_t j = 1; j < e; ++j)
            p_pow *= p;
        phi *= term * p_pow;
    }
    return (uint64_t)phi;
}

/* Return true iff x can be expressed as a sum of two integer squares.  */
static bool is_sum_two_squares(uint64_t x)
{
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64(x, primes, exps, &cnt);

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        if ((p & 3U) == 3U && (e & 1U))
            return false;          /* prime ≡3 (mod 4) with odd exponent */
    }
    return true;
}

/* A333909: nth integer k with phi(k) a sum of two squares. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    uint64_t count = 0;
    uint64_t k = 1;

    while (count < (uint64_t)n) {
        uint64_t phi = phi_uint64(k);
        if (is_sum_two_squares(phi)) {
            ++count;
            if (count == (uint64_t)n)
                return (seq_t)k;
        }
        ++k;
    }
    /* Should never reach here for valid n */
    return 0;
}
