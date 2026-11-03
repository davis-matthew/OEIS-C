/*
 * A293898  Sum of proper divisors of n of the form 3k+2.
 * https://oeis.org/A293898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum of proper divisors of n that are congruent to 2 (mod
 *            3)
 * Range:     u8 n <= 279, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 121 terms through n = 9172 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t e = n / d;
            if (d != n && (d % 3 == 2))
                sum += (seq_t)d;
            if (e != d && e != n && (e % 3 == 2))
                sum += (seq_t)e;
        }
    }
    return sum;
}
