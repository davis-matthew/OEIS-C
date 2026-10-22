/*
 * A353653  Unique monotonic sequence of positive integers satisfying a(a(n))
 *          = k*(n-1) + 3, where k = 6.
 * https://oeis.org/A353653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   let i be such that the block of length 2*6^i contains n. Let
 *            start_i = 1 + (2*(6^i - 1))/5. If n <= start_i + 6^i - 1 then
 *            a(n) = n + 6^i; otherwise a(n) = 6*(n - 6^i) - 3
 * Range:     u8 n <= 79, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t start = (seq_t)1;
    seq_t pow6  = (seq_t)1;
    for (;;) {
        seq_t block_len = pow6 * 2;
        seq_t block_end = start + block_len - 1;
        if (n_seq <= block_end) {
            break;
        }
        start = block_end + 1;
        pow6  = pow6 * 6;
    }
    seq_t threshold = start + pow6 - 1;
    if (n_seq <= threshold) {
        return n_seq + pow6;
    } else {
        return 6 * (n_seq - pow6) - 3;
    }
}
