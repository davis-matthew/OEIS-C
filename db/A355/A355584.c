/*
 * A355584  a(n) is the sum of the 5-smooth divisors of n.
 * https://oeis.org/A355584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = sum of the 5‑smooth divisors of n * Multiplicative:
 *            a(p^e) = (p^(e+1)-1)/(p-1) if p ≤ 5, else 1. * Equivalent: a(n)
 *            = (2^(α+1)-1)*(3^(β+1)-1)*(5^(γ+1)-1) / 8 * where α,β,γ are the
 *            exponents of 2,3,5 in the prime factorisation of n
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Count exponents of 2, 3 and 5 */
    idx_t e2 = 0, e3 = 0, e5 = 0;
    idx_t tmp = n;

    while ((tmp & 1ULL) == 0) {
        e2++;
        tmp >>= 1;
    }

    while (tmp % 3ULL == 0) {
        e3++;
        tmp /= 3;
    }

    while (tmp % 5ULL == 0) {
        e5++;
        tmp /= 5;
    }

    /* Compute the geometric‑series numerators */
    seq_t s2 = big_pow((seq_t)2, (seq_t)(e2 + 1)) - 1;
    seq_t s3 = big_pow((seq_t)3, (seq_t)(e3 + 1)) - 1;
    seq_t s5 = big_pow((seq_t)5, (seq_t)(e5 + 1)) - 1;

    /* Cancel the denominator (2‑1)(3‑1)(5‑1)=8 beforehand
       to keep intermediate values small and exact. */
    s3 /= 2;   /* (3^(β+1)-1) is always even */
    s5 /= 4;   /* (5^(γ+1)-1) is always divisible by 4 */

    seq_t res = s2;
    res *= s3;
    res *= s5;

    return res;
}
