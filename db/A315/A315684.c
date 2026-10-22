/*
 * A315684  Coordination sequence Gal.5.291.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*n + 1 + C(n) where C(n) is the number of indices k (1 ≤
 *            k ≤ n) * with the increment a(k)-a(k-1)=6. * For this tiling
 *            C(n) equals * 0 if n=0 or n=1, * 1 if 2 ≤ n < 9, * 1 +
 *            ⌊(n-8)/10⌋·4 + min((n-8) mod 10, 4) if n ≥ 9. * Thus for n ≥ 9
 *            the formula becomes * a(n) = 5*n + 2 + ⌊(n-8)/10⌋·4 +...
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;          /* a(0) = 1 */
    if (n == 1) return 6;          /* a(1) = 6 */

    if (n < 9)                     /* 2 ≤ n < 9 */
        return (seq_t)5 * n + 2;    /* a(n) = 5*n + 2 */

    /* n ≥ 9 */
    idx_t len     = n - 8;          /* number of indices from 9 to n inclusive   */
    idx_t cycles  = len / 10;       /* full blocks of 10 indices each with 4 "6" */
    idx_t rem     = len % 10;       /* remainder indices after complete blocks   */
    seq_t add = (rem < 4) ? rem : 4;/* additional "6" increments in the incomplete block */

    return (seq_t)5 * n + 2 + (seq_t)cycles * 4 + add;
}
