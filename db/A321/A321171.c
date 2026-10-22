/*
 * A321171  a(n) is the total number of 1's in binary that n shares with the
 *          positive integers less than n.
 * https://oeis.org/A321171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = Sum_{e≥0, n has bit e set} ((n - (n mod 2^{e+1})) / 2 +
 *            (n mod 2^{e+1} - 2^e))
 * Range:     u8 n <= 94, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t e = 0; e < 64; ++e) {
        idx_t bit = ((idx_t)1) << e;
        if (bit > n) break;
        if (n & bit) {
            if (e == 63) {
                /* 2^{64} does not fit in idx_t; handle the top bit separately */
                sum += (seq_t)(n - bit);
            } else {
                idx_t cycle = bit << 1;           /* 2^{e+1} */
                idx_t rem = n % cycle;            /* n mod 2^{e+1} */
                seq_t cnt = ((seq_t)(n - rem) / 2) + (seq_t)(rem - bit);
                sum += cnt;
            }
        }
    }
    return sum;
}
