/*
 * A363155  Expansion of Sum_{k>0} k * x^(3*k-1) / (1 - x^(5*k-2)).
 * https://oeis.org/A363155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/5) * Sum_{d | 5*n-1, d (mod 5) == 3} (d+2)
 * Range:     u8 n <= 436, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* m = 5*n - 1 */
    idx_t m = 5 * n - 1;
    seq_t sum = 0;

    for (idx_t d = 1; d * d <= m; ++d) {
        if (m % d == 0) {
            /* first divisor d */
            if (d % 5 == 3)
                sum += (seq_t)d + 2;
            /* paired divisor d2 */
            idx_t d2 = m / d;
            if (d2 != d && d2 % 5 == 3)
                sum += (seq_t)d2 + 2;
        }
    }
    return sum / 5;
}
