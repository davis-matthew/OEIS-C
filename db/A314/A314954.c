/*
 * A314954  Coordination sequence Gal.4.77.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314954
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + 5*n * minus the number of indices i ≤ n having
 *            residue 1, 2 or 6 modulo 7 * plus the number of indices i ≤ n
 *            having residue 4 modulo 7. * Equivalently: * let r2 = (n>=2) ?
 *            ((n-2)/7 + 1) : 0 // count of i≡2 (mod 7) * r6 = (n>=6) ?
 *            ((n-6)/7 + 1) : 0 // count of i≡6 (mod 7) * r4 = (n>=4)...
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;

    seq_t n_seq = (seq_t)n;          /* cast once for arithmetic on seq_t */

    /* counts of indices with given residues  (using idx_t arithmetic) */
    idx_t cnt_r2 = 0, cnt_r6 = 0, cnt_r4 = 0;
    if (n >= (idx_t)2)
        cnt_r2 = (n - (idx_t)2) / (idx_t)7 + (idx_t)1;
    if (n >= (idx_t)6)
        cnt_r6 = (n - (idx_t)6) / (idx_t)7 + (idx_t)1;
    if (n >= (idx_t)4)
        cnt_r4 = (n - (idx_t)4) / (idx_t)7 + (idx_t)1;

    seq_t cnt_r1 = (seq_t)1;          /* i = 1 is always counted when n ≥ 1 */

    seq_t res = (seq_t)1
                + (seq_t)5 * n_seq
                - ((seq_t)cnt_r1 + (seq_t)cnt_r2 + (seq_t)cnt_r6)
                + (seq_t)cnt_r4;

    return res;
}
