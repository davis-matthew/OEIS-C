/*
 * A346758  a(n) = Sum_{d|n} mu(n/d) * floor(d^2/4).
 * https://oeis.org/A346758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} mu(n/d) * floor(d^2/4) For n >= 3, a(n) =
 *            J_2(n)/4 where J_2(n) = ∏_{p^e||n} p^(2e-2)*(p^2-1). Edge cases:
 *            a(1)=0, a(2)=1
 * Range:     u8 n <= 16, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer power for seq_t */
static seq_t _pow_uint(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > (idx_t)0) {
        if (exp & (idx_t)1) result *= base;
        base *= base;
        exp >>= (idx_t)1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)0;
    if (n == (idx_t)2) return (seq_t)1;

    idx_t tmp = n;
    seq_t prod = (seq_t)1;

    for (idx_t p = (idx_t)2; p <= tmp / p; ++p) {
        if (tmp % p == (idx_t)0) {
            idx_t e = (idx_t)0;
            while (tmp % p == (idx_t)0) {
                tmp /= p;
                ++e;
            }
            idx_t exp = (idx_t)(2 * e - 2);
            seq_t term = (seq_t)1;
            if (exp > (idx_t)0)
                term = _pow_uint((seq_t)p, exp);
            seq_t p2minus1 = (seq_t)p * (seq_t)p - (seq_t)1;
            term *= p2minus1;
            prod *= term;
        }
    }

    if (tmp > (idx_t)1) {            /* remaining prime factor with exponent 1 */
        seq_t p = tmp;
        seq_t p2minus1 = p * p - (seq_t)1;
        prod *= p2minus1;
    }

    return prod / (seq_t)4;          /* integer division, result is exact */
}
