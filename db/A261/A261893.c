/*
 * A261893  a(n) = (n+1)^3 - n^2.
 * https://oeis.org/A261893
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)^3 - n^2
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 1624, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t np1   = n_seq + 1;
    seq_t cube  = np1 * np1 * np1;   /* (n+1)^3   */
    seq_t sqr   = n_seq * n_seq;     /* n^2       */
    return cube - sqr;
}
