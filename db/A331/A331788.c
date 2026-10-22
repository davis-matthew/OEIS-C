/*
 * A331788  a(n) is the smallest m such that for any N, at least one of S(N),
 *          S(N+1), ..., S(N+m-1) is divisible by n, where S(N) is the sum of
 *          digits of N.
 * https://oeis.org/A331788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 10^⌊(n-1)/9⌋ * (2*t - gcd(t,9) + 1) - 1, where t = n -
 *            9*⌊(n-1)/9⌋ (1 ≤ t ≤ 9)
 * Range:     u8 n <= 19, u16 n <= 39, u32 n <= 84, u64 n <= 171,
 *            u128 n <= 343, big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Handle the trivial case if called with n == 0 (outside OEIS offset). */
    if (n == 0)
        return (seq_t)0;

    /* s = ⌊(n-1)/9⌋, t = n - 9*s ensures 1 ≤ t ≤ 9. */
    idx_t s = (n - 1) / 9;
    idx_t t = n - 9 * s;          /* 1 <= t <= 9 */

    /* Compute 10^s as a seq_t using the utility function. */
    seq_t pow10s = pow10(s);

    /* Compute gcd(t,9).  For t in 1..9 this is trivial. */
    int g = 1;
    if (t == 9)          g = 9;
    else if (t == 3 || t == 6) g = 3;

    /* Evaluate the multiplier 2*t - g + 1 as a seq_t. */
    seq_t multiplier = (seq_t)(2 * t - g + 1);

    /* Final result: pow10s * multiplier − 1. */
    return pow10s * multiplier - (seq_t)1;
}
