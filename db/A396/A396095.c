/*
 * A396095  O.g.f. Sum_{n>=0} n^n*x^n/(1 + n*x)^(n-1).
 * https://oeis.org/A396095
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((n+1)*(3*n + 10)/24) * n! for n >= 2; a(0) = 1, a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0 || n == 1) return (seq_t)1;

    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i) fact *= (seq_t)i;

    /* (n+1)*(3*n + 10) */
    seq_t num = (seq_t)(n + 1) * (seq_t)(3*n + 10);

    /* For n >= 4 the factorial is divisible by 24, so we perform the division
       before the multiplication to avoid overflow on 64‑bit builds.  For n = 2,3
       the values are tiny and the straightforward formula fits in the
       underlying type. */
    if (n >= 4) {
        seq_t fact_div = fact / (seq_t)24;
        return fact_div * num;
    } else {
        return fact * num / (seq_t)24;
    }
}
