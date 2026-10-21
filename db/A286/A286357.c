/*
 * A286357  One more than the exponent of the highest power of 2 dividing
 *          sigma(n): a(n) = A001511(A000203(n)).
 * https://oeis.org/A286357
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = v₂(σ(n)) + 1 where σ(n) is the sum of divisors of n
 * Range:     u8 n <= 18, u16 n <= 3948, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Compute σ(n) by factorisation */
    uint64_t temp = (uint64_t)n;        /* match for uint64_t helpers */
    int e2 = 0;
    while ((temp & 1ULL) == 0) {        /* while divisible by 2 */
        ++e2;
        temp >>= 1;                    /* divide by 2 */
    }

    seq_t sigma = 1;

    /* contribution from the power of two */
    if (e2 > 0) {
        seq_t t = pow_uint(2ULL, (idx_t)(e2 + 1)) - 1;    /* 2^(e2+1)-1 */
        sigma *= t;
    }

    /* factorise the odd part */
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                ++e;
                temp /= p;
            }
            seq_t t1 = pow_uint(p, (idx_t)(e + 1));         /* p^(e+1) */
            seq_t term = (t1 - 1) / (p - 1);               /* geometric sum */
            sigma *= term;
        }
    }

    /* remaining prime factor if any */
    if (temp > 1) {
        seq_t t1 = pow_uint(temp, 2) - 1;                 /* p^2-1 */
        seq_t term = t1 / (temp - 1);
        sigma *= term;
    }

    /* compute v₂(σ(n)) + 1 */
    int cnt = 0;
    seq_t val = sigma;
    while ((val & 1) == 0) {
        val >>= 1;
        ++cnt;
    }
    return (seq_t)(cnt + 1);
}
