/*
 * A377301  G.f.: Sum_{k>=1} k * x^(k*(4*k-3)) / (1 - x^k).
 * https://oeis.org/A377301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k | n, n/k >= 4k-3} k (coefficients of G.f.:
 *            Sum_{k>=1} k * x^(k*(4k-3)) / (1 - x^k))
 * Range:     u8 n <= 5039, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            /* divisor d */
            if (n / d >= 4 * d - 3)
                sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d) {
                /* divisor d2 = n/d */
                if (n / d2 >= 4 * d2 - 3)
                    sum += (seq_t)d2;
            }
        }
    }
    return sum;
}
