/*
 * A311586  Coordination sequence Gal.4.58.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311586
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, for n >= 1: a(n) = 4*n + 2*cnt, where cnt = 0 if n <
 *            4, cnt = ((n-4)/7) + 1 if n >= 4. This is equivalent to a(0) =
 *            1, a(1)=4, a(2)=8, a(3)=12, and for n >= 4: a(n) = 4*n +
 *            2*floor((n-4)/7) + 2
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    seq_t n_seq = (seq_t)n;
    if (n < 4)                /* 1 <= n <= 3 */
        return 4 * n_seq;
    /* n >= 4 */
    seq_t cnt = ((seq_t)(n - 4)) / 7 + 1;
    return 4 * n_seq + 2 * cnt;
}
