/*
 * A315360  Coordination sequence Gal.6.668.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315360
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*n - offset(n) a(0) = 1 offset(n) is: n % 6 == 0 or 3 :
 *            0 n % 6 == 1 : 4*((n-1)/6) n % 6 == 2 : 2 + 5*((n-2)/6) n % 6 ==
 *            4 : 3 + 5*((n-4)/6) n % 6 == 5 : 4 + 4*((n-5)/6)
 * Range:     u8 n <= 44, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    seq_t base = (seq_t)n * 6;
    idx_t r = n % 6;
    idx_t offset;
    switch (r) {
    case 0: offset = 0; break;
    case 1: offset = 4 * ((n - 1) / 6); break;
    case 2: offset = 2 + 5 * ((n - 2) / 6); break;
    case 3: offset = 0; break;
    case 4: offset = 3 + 5 * ((n - 4) / 6); break;
    case 5: offset = 4 + 4 * ((n - 5) / 6); break;
    default: offset = 0; break;  /* unreachable */
    }
    return base - (seq_t)offset;
}
