/*
 * A314758  Coordination sequence Gal.6.193.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314758
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(14*n/3) + d[n mod 12], where d =
 *            {0,1,0,-1,1,0,0,1,0,1,1,0}
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;                   /* a(0) = 1 */

    /* Compute floor(14*n/3) without overflow:
       floor(14*n/3) = 14*(n/3) + floor(14*(n%3)/3)   */
    seq_t q = (seq_t)n / 3;                  /* n/3   */
    seq_t r = (seq_t)n % 3;                  /* n%3   */
    seq_t res = q * 12 + q * 2;              /* 14*q  */
    res += (r * 14) / 3;                     /* + floor(14*r/3) */

    /* Adjustment depending on n mod 12 */
    static const int d[12] = {0, 1, 0, -1, 1, 0, 0, 1, 0, 1, 1, 0};
    int delta = d[n % 12];
    if (delta >= 0)
        res += (seq_t)delta;
    else
        res -= (seq_t)(-delta);

    return res;
}
