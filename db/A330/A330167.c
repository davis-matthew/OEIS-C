/*
 * A330167  Length of the longest run of 1's in the ternary expression of n.
 * https://oeis.org/A330167
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = length of longest run of 1's in ternary representation of
 *            n
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t max_run = 0;
    seq_t cur_run = 0;
    while (n > 0) {
        uint64_t digit = n % 3;
        if (digit == 1) {
            cur_run++;
            if (cur_run > max_run) {
                max_run = cur_run;
            }
        } else {
            cur_run = 0;
        }
        n /= 3;
    }
    return max_run;
}
