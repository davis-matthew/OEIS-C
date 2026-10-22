/*
 * A344405  a(n) = Sum_{d|n} (n/d) * floor(n/d^2).
 * https://oeis.org/A344405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (n/d) * floor(n/d^2)
 * Range:     u8 n <= 15, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            idx_t d2 = n / d;
            /* term for divisor d */
            idx_t flo1 = n / (d * d);
            result += (seq_t)(n / d) * (seq_t)flo1;

            if (d2 != d) {
                /* term for complementary divisor d2 */
                idx_t flo2;
                if (d2 > n / d2) {
                    flo2 = 0;
                } else {
                    flo2 = n / (d2 * d2);
                }
                /* n/d2 == d for the complementary divisor */
                result += (seq_t)d * (seq_t)flo2;
            }
        }
    }
    return result;
}
