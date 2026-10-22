/*
 * A304833  a(n) = 3*n^2 + 38*n - 76 (n>=2).
 * https://oeis.org/A304833
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n^2 + 38*n - 76
 * Range:     u8 n <= 5, u16 n <= 141, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t val = (seq_t)3 * sn * sn;   /* 3*n^2 */
    val += (seq_t)38 * sn;            /* + 38*n */
    val -= (seq_t)76;                 /* - 76 */
    return val;
}
