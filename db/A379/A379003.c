/*
 * A379003  Ordinal transform of A132741, where A132741 is the largest divisor
 *          of n having the form 2^i*5^j. a(0) = 0 by convention.
 * https://oeis.org/A379003
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Let v be the largest divisor of n that is of the form 2^i * 5^j;
 *            equivalently, v = n / m where m is obtained by dividing n by all
 *            factors of 2 and 5. Then a(n) = #{t ≤ n / v : gcd(t,10) = 1} = m
 *            - ⌊m/2⌋ - ⌊m/5⌋ + ⌊m/10⌋ (m = n / v) a(0) = 0 by convention
 * Range:     u8 n <= 302, u16 n <= 9450, u32 n <= 9450, u64 n <= 9450,
 *            u128 n <= 9450, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* remove all factors 2 and 5 from n */
    idx_t rest = n;
    while ((rest & 1ULL) == 0)  /* rest % 2 == 0 */
        rest >>= 1;
    while (rest % 5 == 0)
        rest /= 5;

    /* v is the 2^i * 5^j part */
    idx_t v = n / rest;

    /* m = n / v  (integer division, v divides n exactly) */
    seq_t m = (seq_t)n / (seq_t)v;

    /* Count numbers ≤ m that are coprime to 10
       using inclusion–exclusion for divisibility by 2 and 5. */
    seq_t a_val = m - m / 2 - m / 5 + m / 10;

    return a_val;
}
