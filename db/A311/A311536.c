/*
 * A311536  Coordination sequence Gal.6.120.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311536
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*n + g(n) where g(n) = 1,0,0,0 for n < 4, g(n) =
 *            6*floor((n-4)/12) + ( (n-4)%12 < 6 ? ((n-4)%12 + 1) : 6 ) for n
 *            ≥ 4
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* first four terms */
    static const seq_t first[4] = { (seq_t)1, (seq_t)4, (seq_t)8, (seq_t)12 };
    if (n < 4)
        return first[n];

    /* n >= 4 */
    idx_t k   = n - (idx_t)4;        /* shift index */
    idx_t q   = k / (idx_t)12;      /* block number */
    idx_t r   = k % (idx_t)12;      /* position within block */
    seq_t f;

    if (r < (idx_t)6)
        f = (seq_t)(6 * q + r + (idx_t)1); /* increasing part */
    else
        f = (seq_t)(6 * q + (idx_t)6);     /* plateau part */

    return (seq_t)4 * n + f;
}
