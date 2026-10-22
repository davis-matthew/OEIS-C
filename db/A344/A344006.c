/*
 * A344006  a(n) = m*(m+1)/n, where A344005(n) is the smallest number m such
 *          that n divides m*(m+1).
 * https://oeis.org/A344006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = m*(m+1)/n where m is the smallest positive integer with n
 *            | m*(m+1)
 * Range:     u8 n <= 16, u16 n <= 520, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A344006: a(n) = smallest m≥1 such that n divides m*(m+1), then a(n) = m*(m+1)/n */
seq_t a(idx_t n) {
    /* cast n to seq_t for arithmetic */
    seq_t n_seq = (seq_t)n;
    seq_t m = 1;  /* minimal positive m */
    while (1) {
        seq_t prod = m * (m + 1);
        if (prod % n_seq == 0) {
            return prod / n_seq;
        }
        ++m;
    }
}
