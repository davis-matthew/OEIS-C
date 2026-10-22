/*
 * A304993  a(n) = n*(n + 1)*(7*n + 5)/6.
 * https://oeis.org/A304993
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n + 1)*(7*n + 5)/6
 * Range:     u8 n <= 2, u16 n <= 20, u32 n <= 905, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t term = nn * (nn + (seq_t)1);
    term = term * ((seq_t)7 * nn + (seq_t)5);
    term /= (seq_t)6;
    return term;
}
