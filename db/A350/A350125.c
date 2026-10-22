/*
 * A350125  a(n) = Sum_{k=1..n} k^2 * floor(n/k)^n.
 * https://oeis.org/A350125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} k^2 * floor(n/k)^n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Sum of squares 1^2+...+m^2 */
static inline seq_t sum_sq(idx_t m)
{
    seq_t mm = (seq_t)m;
    return mm * (mm + 1) * (2 * mm + 1) / 6;
}

/* Power by squaring: base^exp (exp is unsigned) */
static inline seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t res = 1;
    while (exp)
    {
        if (exp & 1) res *= base;
        exp >>= 1;
        if (exp) base *= base;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t k = 1; k <= n; )
    {
        idx_t v   = n / k;          /* floor(n/k) */
        idx_t kmax = n / v;         /* largest k with same v */
        seq_t sumk2 = sum_sq(kmax) - sum_sq(k - 1);
        seq_t v_pow = pow_seq((seq_t)v, n);
        result += sumk2 * v_pow;
        k = kmax + 1;
    }
    return result;
}
