/*
 * A313801  Coordination sequence Gal.5.291.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*n + number of integers k with 0 ≤ k ≤ n and k mod 10 ∈
 *            {4,5,6,7}
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A313801 */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* a(0) = 1 */

    /* Count numbers k ∈ [0,n] whose remainder mod 10 is 4,5,6 or 7 */
    idx_t cycles = (n + 1) / 10;     /* complete groups of 10 */
    idx_t rem    = (n + 1) % 10;     /* remaining part of the last group */
    seq_t q = (seq_t)cycles * 4;     /* four qualifying numbers per full group */

    if (rem > 4) {
        idx_t add = rem - 4;        /* how many of 4,5,6,7 lie in the partial group */
        if (add > 4) add = 4;       /* at most four */
        q += (seq_t)add;
    }

    return (seq_t)5 * n + q;        /* 5*n counts the baseline of 5 every step,
                                       plus the extra +1 for each qualifying k */
}
