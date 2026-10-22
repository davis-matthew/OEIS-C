/*
 * A312893  Coordination sequence Gal.5.95.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312893
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=4. For n>=2 a(n)=4*n + 1 + floor((n-2)/9) + (n>=8 ?
 *            floor((n-8)/9)+1 : 0) (derived from the blockwise pattern of
 *            d(n)=a(n)-4n)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;

    /* n >= 2 */
    idx_t N = n - 2;                    /* N >= 0 */
    idx_t inc1 = N / 9;                 /* floor((n-2)/9)   */
    idx_t inc2 = 0;
    if (n >= 8)                         /* n-8 >= 0  */
        inc2 = (n - 8) / 9 + 1;         /* floor((n-8)/9)+1 */

    idx_t d = 1 + inc1 + inc2;           /* d = a(n) - 4*n  */
    return ( (seq_t)4 * (seq_t)n ) + (seq_t)d;
}
