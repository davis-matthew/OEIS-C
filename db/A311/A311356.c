/*
 * A311356  Coordination sequence Gal.6.219.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311356
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n≥1, * a(n) = 4*n + 2*count{ k∈[1,n] | k mod 11 ∈
 *            {5,7} } * i.e. each step adds 4, plus 2 when the index is 5 or 7
 *            modulo 11
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    /* Count of terms with k mod 11 == 5 or 7 up to n */
    idx_t blocks = n / 11;          /* full 11‑term blocks     */
    idx_t rem    = n % 11;          /* remaining terms        */
    seq_t extra = 4 * (seq_t)blocks;    /* 2 per block → 4*blocks */

    if (rem >= 5)   /* term 5 in the block contributes +2 */
        extra += 2;
    if (rem >= 7)   /* term 7 in the block contributes +2 */
        extra += 2;

    return 4 * (seq_t)n + extra;
}
