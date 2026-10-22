/*
 * A315689  Coordination sequence Gal.6.624.6 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315689
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((11*n + r(n)) / 2) where r(n) is a periodic
 *            adjustment: r(0) = 2 for n > 0: r(n) depends only on n mod 12 n
 *            mod 12 : 0 1 2 3 4 5 6 7 8 9 10 11 r(n) : 0 1 2 1 0 1 0 -1 0 -1
 *            -2 -1 This yields the coordination sequence for Gal.6.624.6
 * Range:     u8 n <= 46, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Determine the adjustment r(n) from n mod 12. */
    idx_t m = n % 12;
    int r;
    switch (m) {
        case 0:  r = 0; break;  /* n mod 12 == 0 */
        case 1:  r = 1; break;
        case 2:  r = 2; break;
        case 3:  r = 1; break;
        case 4:  r = 0; break;
        case 5:  r = 1; break;
        case 6:  r = 0; break;
        case 7:  r = -1; break;
        case 8:  r = 0; break;
        case 9:  r = -1; break;
        case 10: r = -2; break;
        case 11: r = -1; break;
        default: r = 0; /* unreachable */
    }

    /* Compute floor((11*n + r) / 2) safely using signed arithmetic. */
    long long numerator = (long long)11 * (long long)n + (long long)r;
    seq_t result = (seq_t)(numerator / 2);

    return result;
}
