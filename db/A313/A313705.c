/*
 * A313705  Coordination sequence Gal.6.452.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313705
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*n - floor((n+1)/5) if n mod 5 ∈ {1,4}; a(n) = 5*n
 *            otherwise; a(0) = 1
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the special case n = 0 */
    if (n == 0)
        return (seq_t)1;

    /* Compute 5*n in seq_t */
    seq_t result = (seq_t)5 * (seq_t)n;

    /* Determine if a deficit is applied */
    unsigned r = (unsigned)(n % 5);
    if (r == 1 || r == 4) {
        /* Compute floor((n+1)/5) */
        idx_t q = n / 5;               /* q = floor(n/5)   */
        seq_t d = (seq_t)q;            /* floor((n+1)/5) for r==1 */
        if (r == 4)
            d += 1;                    /* add one more for r==4 */
        result -= d;                  /* apply the deficit */
    }
    return result;
}
