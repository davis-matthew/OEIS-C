/*
 * A315490  Coordination sequence Gal.4.141.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315490
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*n + 1 + |{ i ∈ [4,n] : i mod 7 ∈ {0,1,4} }|
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 4 the sequence is just a(n) = 5*n + 1 */
    if (n < 4)
        return (seq_t)5 * (seq_t)n + 1;

    /* Count the indices i ≥ 4 with i mod 7 in {0,1,4} */
    idx_t k   = n - 4;            /* distance from the first special index */
    idx_t cnt = (k + 1) / 7;      /* full 7‑blocks, each contributes 3 */
    idx_t rem = (k + 1) % 7;      /* remaining elements in the last block */

    /* Convert to seq_t for the final count */
    seq_t res = (seq_t)3 * (seq_t)cnt;

    /* Count extra indices in the partial block with residues 0,3,4 */
    if (rem > 0) res += 1;        /* residue 0 at r = 0  */
    if (rem > 3) res += 1;        /* residue 3 at r = 3  */
    if (rem > 4) res += 1;        /* residue 4 at r = 4  */

    return (seq_t)5 * (seq_t)n + 1 + res;
}
