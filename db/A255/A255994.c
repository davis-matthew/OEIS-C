/*
 * A255994  Number of length n+3 0..1 arrays with at most one downstep in
 *          every n consecutive neighbor pairs.
 * https://oeis.org/A255994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^3 + 9*n^2 + 62*n + 24) / 6
 * Range:     u8 n <= 2, u16 n <= 37, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* offset is 1, not used below 1 */
    seq_t nn = (seq_t)n;
    seq_t num = nn * nn * nn + 9 * nn * nn + 62 * nn + 24;
    return num / 6;
}
