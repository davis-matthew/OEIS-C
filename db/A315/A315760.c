/*
 * A315760  Coordination sequence Gal.6.638.6 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315760
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Coordination sequence A315760:
   For n ‖ using the following piecewise formulas:
      if n == 0                     : a(n) = 1
      if n % 6 == 0                 : a(n) = 34 * (n / 6)
      if n % 6 == 1 or 5            : a(n) = base[r] + 68 * floor((n-r)/12) + 33 * ((n-r)/6 % 2)
                                    where base[1] = 6, base[5] = 29
      if n % 6 == 2,3,4             : a(n) = base[r] + 32 * ((n-r)/6) + 4 * floor(((n-r)/6)/2)
                                    where base[2] = 12, base[3] = 18, base[4] = 24
   Here r = n % 6 and m = (n - r) / 6.  All computations are performed in seq_t
   to preserve precision for arbitrarily large integer types.
*/

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;          /* a(0) = 1 */

    /* Residue and quotient */
    idx_t r = n % 6u;
    idx_t m = (n - r) / 6u;                /* m = floor(n/6) for r == 0, else floor((n-r)/6) */

    switch (r)
    {
        case 0u:         /* multiples of 6, n >= 6 */
            return (seq_t)34 * (seq_t)m;
        case 1u:        /* r == 1 */
            return (seq_t)6
                 + (seq_t)(68 * (m / 2u) + 33 * (m % 2u));
        case 2u:        /* r == 2 */
            return (seq_t)12
                 + (seq_t)(32 * m + 4 * (m / 2u));
        case 3u:        /* r == 3 */
            return (seq_t)18
                 + (seq_t)(32 * m + 4 * (m / 2u));
        case 4u:        /* r == 4 */
            return (seq_t)24
                 + (seq_t)(32 * m + 4 * (m / 2u));
        case 5u:        /* r == 5 */
            return (seq_t)29
                 + (seq_t)(68 * (m / 2u) + 33 * (m % 2u));
        default:        /* unreachable */
            return (seq_t)0;
    }
}
