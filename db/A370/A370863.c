/*
 * A370863  Numbers m such that c(0) > c(1) < c(2), where c(k) = number of k's
 *          in the ternary representation of m.
 * https://oeis.org/A370863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer m such that the number of 0's
 *            in the ternary representation of m exceeds the number of 1's,
 *            and the number of 1's is strictly less than the number of 2's
 * Range:     u8 n <= 45, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t m = 1;             /* start with the first positive integer */

    while (1) {
        unsigned c0 = 0, c1 = 0, c2 = 0;
        idx_t t = m;

        /* count digits of m in base 3 */
        while (t > 0) {
            unsigned d = t % 3;
            if (d == 0) ++c0;
            else if (d == 1) ++c1;
            else ++c2;
            t /= 3;
        }

        if (c0 > c1 && c1 < c2) {
            ++count;
            if (count == n)
                return (seq_t)m;   /* found the n-th term */
        }
        ++m;
    }
}
