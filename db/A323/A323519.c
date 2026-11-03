/*
 * A323519  a(n) is the number of ways to fill a square matrix with the
 *          multiset of prime factors of n, if the number of prime factors
 *          (counted with multiplicity) is a perfect square, and a(n) = 0
 *          otherwise.
 * https://oeis.org/A323519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if bigomega(n) is not a perfect square; otherwise a(n)
 *            = (bigomega(n))! / product over prime exponents of
 *            factorial(exponent)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Validated: partial, pass, 86 terms through n = 86 (2026-11-01)
 * Helpers:   bigomega, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/bigomega.h"
#include "../utils/factor_u64.h"

/* Integer square root (floor) of a non‑negative seq_t value. */
static seq_t isqrt_seq(seq_t v)
{
    seq_t lo = 0, hi = v;
    while (lo <= hi) {
        seq_t mid = lo + ((hi - lo) >> 1);
        seq_t sq  = mid * mid;
        if (sq == v) return mid;
        if (sq < v) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return hi;
}

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

seq_t a(idx_t n)
{
    /* number of prime factors (with multiplicity) */
    seq_t omega = bigomega(n);
    /* integer square root of omega */
    seq_t s = isqrt_seq(omega);

    /* return zero if omega is not a perfect square */
    if (s * s != omega)
        return 0;

    /* multinomial coefficient (omega)! / ∏ (exponent_i)! */
    seq_t result = factorial((idx_t)omega);

    /* factorize n to obtain the exponents */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    for (size_t i = 0; i < cnt; ++i) {
        seq_t denom = factorial((idx_t)exps[i]);
        result /= denom;
    }

    return result;
}
