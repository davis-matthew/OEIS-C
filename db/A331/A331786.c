/*
 * A331786  a(n) is the largest m such that there exists N such that none of
 *          S(N), S(N+1), ..., S(N+m-1) is divisible by n, where S(N) is the
 *          sum of digits of N.
 * https://oeis.org/A331786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 10^s * (2*t - gcd(t,9) + 1) - 2, where n = 9*s + t, 1 ≤ t
 *            ≤ 9
 * Range:     u8 n <= 19, u16 n <= 39, u32 n <= 84, u64 n <= 171,
 *            u128 n <= 343, big any n
 * Helpers:   gcd, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Handle the offset start (n = 1 → a(1) = 0) and guard against n == 0 */
    if (n == 0) return (seq_t)0;

    /* Decompose n into n = 9*s + t, with 1 ≤ t ≤ 9 */
    idx_t s = (n - 1) / 9U;
    idx_t t = n - 9U * s;           /* t is in [1,9] */

    /* Compute gcd(t,9) */
    seq_t g = gcd((seq_t)t, (seq_t)9U);

    /* Compute the multiplier (2*t - g + 1) */
    int multiplier = 2 * (int)t - (int)g + 1;   /* fits in int */

    /* Compute 10^s in seq_t */
    seq_t pow10s = pow10(s);

    /* Compute the product and subtract 2 */
    seq_t result = pow10s * (seq_t)multiplier - (seq_t)2U;

    return result;
}
