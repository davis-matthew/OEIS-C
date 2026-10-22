/*
 * A315721  Coordination sequence Gal.6.667.6 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315721
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n > 0 let k = floor(n/3). if n % 3 == 0: a = 6 * n
 *            else if n % 3 == 1 and k odd : a = 6 * n - 3 * (k + 1) else if n
 *            % 3 == 2 and k even : a = 6 * n - 3 * k else: a = 6 * n
 * Range:     u8 n <= 42, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t np = (seq_t)n;
    seq_t sixn = np * 6;

    switch (n % 3)
    {
        case 0:
            return sixn;
        case 1:
        {
            idx_t k = n / 3;
            if (k & 1)   /* k odd */
                return sixn - 3 * (seq_t)(k + 1);
            else
                return sixn;
        }
        case 2:
        {
            idx_t k = n / 3;
            if ((k & 1) == 0)   /* k even */
                return sixn - 3 * (seq_t)k;
            else
                return sixn;
        }
    }
    return sixn;   /* unreachable, keeps compiler happy */
}
