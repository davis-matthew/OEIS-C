/*
 * A342253  a(n) = (n-6)*sqrt((n-5)^2) + 2*n + 31.
 * https://oeis.org/A342253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-6)*sqrt((n-5)^2) + 2*n + 31
 * Range:     u8 n <= 19, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Piecewise: a(n) = -n^2 + 13*n + 1 for n <= 5,
 *            a(n) =  n^2 -  9*n + 61 for n >= 5. */

seq_t a(idx_t n) {
    /* Use uint64_t for the index, seq_t for the sequence value. */
    if (n <= 5) {
        seq_t n_seq = (seq_t)n;
        return (seq_t)(13 * n + 1) - n_seq * n_seq;
    } else {
        seq_t n_seq = (seq_t)n;
        return n_seq * n_seq - (seq_t)9 * n_seq + 61;
    }
}
