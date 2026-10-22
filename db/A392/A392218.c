/*
 * A392218  Product of the divisors of phi(n).
 * https://oeis.org/A392218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = product of all divisors of φ(n) = φ(n)^(τ(φ(n))/2) where
 *            τ(m) is the number of divisors of m
 * Range:     u8 n <= 12, u16 n <= 30, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   factor_u64, pow_uint, sqrt_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"
#include "../utils/sqrt_floor.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined for n=0, but guard against it */

    /*--- 1. compute φ(n) -------------------------- */
    uint64_t n64 = (uint64_t)n;
    uint64_t phi = n64;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt = 0;

    factor_u64(n64, primes, exps, &pcnt);
    for (size_t i = 0; i < pcnt; ++i)
        phi -= phi / primes[i];

    /*--- 2. τ(φ(n)) -------------------------------- */
    uint64_t phi64 = phi;
    if (phi64 == 1)          /* product of divisors of 1 is 1 */
        return 1;

    uint64_t fpr[64];
    uint64_t fex[64];
    size_t fcnt = 0;

    factor_u64(phi64, fpr, fex, &fcnt);

    uint64_t tau = 1;
    for (size_t i = 0; i < fcnt; ++i)
        tau *= (fex[i] + 1);

    /*--- 3. product of divisors --------------------- */
    seq_t result;
    if (tau % 2 == 0) {
        result = pow_uint(phi64, tau / 2);
    } else {
        seq_t sqrtphi = sqrt_floor((seq_t)phi64);
        result = sqrtphi * pow_uint(phi64, (tau - 1) / 2);
    }

    return result;
}
