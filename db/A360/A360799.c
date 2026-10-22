/*
 * A360799  Numbers m with m mod 3 = q, q != 2, such that the number of ones
 *          in its base-2 representation is even if q=0 and odd if q=1.
 * https://oeis.org/A360799
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth nonnegative integer m such that * m mod 3 is 0 or
 *            1, and the number of ones in the binary * representation of m is
 *            even when m mod 3 = 0 and odd when * m mod 3 = 1
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t found = 0;
    seq_t m = 0;
    while (1) {
        seq_t q = m % (seq_t)3;          /* m mod 3 */
        /* Parity of popcount(m): 0 = even, 1 = odd */
        int parity = 0;
        seq_t tmp = m;
        while (tmp) {
            parity ^= 1;
            tmp &= tmp - 1;
        }
        if ((q == 0 && parity == 0) || (q == 1 && parity == 1)) {
            if (found == n)
                return m;
            ++found;
        }
        ++m;
    }
}
