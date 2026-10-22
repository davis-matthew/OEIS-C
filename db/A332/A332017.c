/*
 * A332017  a(n) is the sum of the squares of the lengths of the runs of
 *          consecutive equal digits in the binary representation of n.
 * https://oeis.org/A332017
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(n) = sum of squares of the lengths of runs of consecutive
 *            equal digits in the binary representation of n
 * Range:     u8 n <= 8192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Find most‑significant set bit index */
    idx_t msb = 0;
    idx_t tmp = n;
    while (tmp > 1) {
        tmp >>= 1;
        ++msb;
    }

    unsigned char cur_bit = (n >> msb) & 1;
    seq_t run_len = 1;
    seq_t sum = 0;

    for (idx_t i = msb; i > 0; --i) {
        unsigned char next_bit = (n >> (i - 1)) & 1;
        if (next_bit == cur_bit) {
            ++run_len;
        } else {
            sum += run_len * run_len;
            cur_bit = next_bit;
            run_len = 1;
        }
    }

    sum += run_len * run_len;
    return sum;
}
