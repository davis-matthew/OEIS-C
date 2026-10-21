/*
 * A241603  a(n) = Sum_{d|n, d <= 5} d^2 + 5*Sum_{d|n, d>5} d.
 * https://oeis.org/A241603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n, d≤5} d^2 + 5 Σ_{d|n, d>5} d
 * Range:     u8 n <= 23, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t small_sum2 = 0;   /* sum of d^2 for d <= 5 */
    seq_t large_sum  = 0;   /* sum of d for d > 5 */

    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            idx_t d1 = d;
            idx_t d2 = n / d;

            if (d1 <= 5)
                small_sum2 += (seq_t)d1 * (seq_t)d1;
            else
                large_sum += (seq_t)d1;

            if (d1 != d2) {
                if (d2 <= 5)
                    small_sum2 += (seq_t)d2 * (seq_t)d2;
                else
                    large_sum += (seq_t)d2;
            }
        }
    }

    return small_sum2 + (seq_t)5 * large_sum;
}
