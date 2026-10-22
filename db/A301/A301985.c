/*
 * A301985  a(n) = n^2 + 2329*n + 1697.
 * https://oeis.org/A301985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 + 2329*n + 1697
 * Range:     u8 none, u16 n <= 27, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;                  /* cast index to seq_t */
    return tn*tn + (seq_t)2329*tn + (seq_t)1697;
}
