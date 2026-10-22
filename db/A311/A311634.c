/*
 * A311634  Coordination sequence Gal.6.233.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311634
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*n + 2*q, where q = number of indices k in [4,n] with
 *            (k-4)%10 < 4 (i.e. the number of +6 increments after the initial
 *            part of the sequence)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle base cases */
    if (n == 0) return 1;               /* a(0) = 1  */
    if (n <= 3) return 4 * (seq_t)n;    /* a(1)=4, a(2)=8, a(3)=12 */

    /* for n >= 4 */
    idx_t count = n - 3;                /* number of terms in [4, n] inclusive */
    seq_t q = (seq_t)(count / 10) * 4;   /* 4 six‑increments per full block of 10 */
    seq_t r = (seq_t)(count % 10);       /* remaining indices in the last block */
    if (r > 4) r = 4;                    /* count only the first 4 of them as +6 */
    q += r;                              /* total number of +6 increments */

    return 4 * (seq_t)n + 2 * q;         /* final formula */
}
