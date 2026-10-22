/*
 * A315632  Coordination sequence Gal.5.112.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315632
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*n + offset(n) where offset(n) is defined piecewise: -
 *            If n is even (n = 2k): k = n/2 if k == 0 offset = +1 else if k
 *            mod 4 == 1 offset = +2 else if k mod 4 == 3 offset = -2 else
 *            offset = 0 - If n is odd (n = 2k+1): t = (n-1)/4 if t is even
 *            offset = +1 else offset = -1
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)5 * n;   /* 5*n, cast to seq_t first */
    if ((n & 1) == 0) {         /* even n */
        idx_t k = n >> 1;       /* k = n/2 */
        int offset;
        if (k == 0)                     offset =  1;
        else if ((k & 3) == 1)           offset =  2;
        else if ((k & 3) == 3)           offset = -2;
        else                              offset =  0;

        if (offset >= 0)
            return base + (seq_t)offset;
        else
            return base - (seq_t)(-offset);
    } else {                     /* odd n */
        idx_t t = (n - 1) >> 2; /* (n-1)/4 */
        int offset = ((t & 1) == 0) ? 1 : -1;

        if (offset >= 0)
            return base + (seq_t)offset;
        else
            return base - (seq_t)(-offset);
    }
}
