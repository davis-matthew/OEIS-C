/*
 * A315257  Coordination sequence Gal.4.100.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315257
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n > 0, a(n) = 5*n + 1 if n mod 6 = 1, a(n) = 5*n -
 *            1 if n mod 6 = 5, otherwise a(n) = 5*n
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t result = 5 * (seq_t)n;
    switch (n % 6)
    {
        case 1:
            result += 1;
            break;
        case 5:
            result -= 1;
            break;
        default:
            /* no change */
            break;
    }
    return result;
}
