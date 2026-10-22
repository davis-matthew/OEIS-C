/*
 * A315757  Coordination sequence Gal.5.294.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315757
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; a(1..4)=6*n; for n≥5, a(n)=6*n-2*((n-5)/9+1) (integer
 *            division is floor)
 * Range:     u8 n <= 44, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)                     /* a(0) = 1 */
        return (seq_t)1;

    if (n < 5)                      /* a(1) = 6, a(2) = 12, a(3) = 18, a(4) = 24 */
        return (seq_t)6 * n;

    idx_t k = (n - 5) / 9;          /* number of full 9‑term blocks after the first 5 */
    seq_t deficit = (seq_t)2 * (k + 1);
    return (seq_t)6 * n - deficit; /* 6*n minus the calculated deficit */
}
