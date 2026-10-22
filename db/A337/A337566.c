/*
 * A337566  a(n) is the number of possible decompositions of the polynomial n
 *          * (x + x^2 + ... + x^q), where q > 1, into a sum of k polynomials,
 *          not necessarily all different; each of these polynomials is to be
 *          of the form b_1 * x + b_2 * x^2 + ... + b_q * x^q where each b_i
 *          is one of the numbers 1, 2, 3, .....
 * https://oeis.org/A337566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = tau(2*n) - 3 (for n ≥ 2), a(1)=0 where tau(m) is the
 *            number of positive divisors of m
 * Range:     u8 n <= 9269, u16 n <= 9269, u32 n <= 9269, u64 n <= 9269,
 *            u128 n <= 9269, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n is idx_t (uint64_t).  Return 0 for the first term. */
    if (n < 2)
        return (seq_t)0;

    /* Compute tau(2*n).  2*n fits into uint64_t for the intended range. */
    uint64_t m = (uint64_t)n * 2ULL;     /* 2*n */
    seq_t tau = 1;                      /* divisor count accumulator */

    /* Factor out powers of 2. */
    uint64_t cnt = 0;
    while ((m & 1ULL) == 0) {            /* m % 2 == 0 */
        m >>= 1;
        ++cnt;
    }
    if (cnt > 0)
        tau *= (seq_t)(cnt + 1);

    /* Factor the remaining odd part. */
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            cnt = 0;
            while (m % p == 0) {
                m /= p;
                ++cnt;
            }
            tau *= (seq_t)(cnt + 1);
        }
    }

    /* If m > 1 now, it is a prime factor with exponent 1. */
    if (m > 1)
        tau *= (seq_t)2;

    /* a(n) = tau(2*n) - 3.  For n ≥ 2 this is non‑negative. */
    return tau - (seq_t)3;
}
