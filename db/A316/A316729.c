/*
 * A316729  Generalized 30-gonal (or triacontagonal) numbers: m*(14*m - 13)
 *          with m = 0, +1, -1, +2, -2, +3, -3, ...
 * https://oeis.org/A316729
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 135, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = n*(7*n + 13)/2   if n is even;
 *           a(n) = (n + 1)*(7*n - 6)/2   if n is odd.
 * This implements OEIS sequence A316729 – Generalized 30‑gonal numbers.
 */
seq_t a(idx_t n)
{
    if ((n & 1) == 0) {               /* even n */
        seq_t n_seq = (seq_t)n;
        /* n * (7*n + 13) / 2 */
        return n_seq * (7 * n_seq + 13) / 2;
    } else {                         /* odd n */
        seq_t n_seq   = (seq_t)n;
        seq_t n_plus1 = n_seq + 1;
        /* (n + 1) * (7*n - 6) / 2 */
        return n_plus1 * (7 * n_seq - 6) / 2;
    }
}
