/*
 * A370870  Numbers m such that c(0) > c(1) > c(2), where c(k) = number of k's
 *          in the ternary representation of m.
 * https://oeis.org/A370870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th integer m (n ≥ 1) such that in its ternary *
 *            representation the number of 0’s exceeds the number of 1’s, and
 *            the * number of 1’s exceeds the number of 2’s. This
 *            implementation scans * integers in increasing order until the
 *            n‑th qualifying value is * found
 * Range:     u8 n <= 14, u16 n <= 5554, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = 0;       /* current integer to test */
    idx_t found = 0;   /* how many qualifying integers we have seen */

    while (1) {
        /* Count 0’s, 1’s and 2’s in the ternary representation of m */
        unsigned int c0 = 0, c1 = 0, c2 = 0;
        seq_t tmp = m;

        if (tmp == 0) {
            c0 = 1;  /* ternary of 0 is "0" */
        } else {
            while (tmp > 0) {
                seq_t digit = tmp % 3;
                tmp /= 3;
                unsigned int d = (unsigned int)digit; /* d is 0,1,or 2 */
                if (d == 0) ++c0;
                else if (d == 1) ++c1;
                else ++c2;
            }
        }

        /* Check the required inequality */
        if (c0 > c1 && c1 > c2) {
            ++found;
            if (found == n) return m;
        }

        ++m;  /* move to the next integer */
    }
}
