/*
 * A345321  Sum of the divisors of n whose cube does not divide n.
 * https://oeis.org/A345321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, d^3 ∤ n} d (sum of the divisors of n whose cube
 *            does not divide n)
 * Range:     u8 n <= 107, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 85 terms through n = 872 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Test whether d^3 divides n: return true if d^3 does NOT divide n */
static bool cube_not_divides(seq_t n, seq_t d)
{
    seq_t temp = n;
    for (int k = 0; k < 3; ++k) {
        if (temp % d != 0)
            return true;          /* d^3 does NOT divide n */
        temp /= d;
    }
    return false;                 /* d^3 divides n */
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* offset is 1, but guard for 0 */

    seq_t sn = (seq_t)n;
    seq_t sum = 0;

    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            seq_t d1 = (seq_t)i;
            if (cube_not_divides(sn, d1))
                sum += d1;

            idx_t o = n / i;
            if (o != i) {         /* avoid double counting when n is a square */
                seq_t d2 = (seq_t)o;
                if (cube_not_divides(sn, d2))
                    sum += d2;
            }
        }
    }
    return sum;
}
