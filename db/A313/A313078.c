/*
 * A313078  Coordination sequence Gal.5.67.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313078
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1. For n ≥ 1, a(n) = a(n-1) + d(n) where d(1) = 3. For n
 *            ≥ 2 let q = n / 5, r = n % 5. If r == 3 d(n) = 5; else if r == 2
 *            d(n) = 5 - (q & 1); else if r == 4 d(n) = 4 + (q & 1); else (r
 *            == 0 or r == 1) d(n) = 5 + ((q & 1) ? 1 : -1);
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t sum = (seq_t)1;
    for (idx_t k = 1; k <= n; ++k)
    {
        int d;
        if (k == 1)
            d = 3;
        else
        {
            idx_t q = k / 5;
            idx_t r = k % 5;
            switch (r)
            {
                case 0:
                case 1:
                    d = 5 + ((q & 1) ? 1 : -1);
                    break;
                case 2:
                    d = 5 - (int)(q & 1);
                    break;
                case 3:
                    d = 5;
                    break;
                case 4:
                    d = 4 + (int)(q & 1);
                    break;
                default:
                    d = 0; /* unreachable */
                    break;
            }
        }
        sum += (seq_t)d;
    }
    return sum;
}
