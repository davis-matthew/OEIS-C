/*
 * A314836  Coordination sequence Gal.6.131.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314836
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((9*n + t(n))/2) * where * t(0) = 1 * t(n) = 0 if n
 *            is even * t(n) = +1 if n is odd and floor((n-1)/6) is even *
 *            t(n) = -1 if n is odd and floor((n-1)/6) is odd * This matches
 *            the OEIS sequence A314836
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;   /* a(0) = 1 */

    /* compute 9*n as an unsigned value (will fit in idx_t for the intended ranges) */
    idx_t base = n * (idx_t)9;

    /* determine the correction t for odd n */
    idx_t t = 0;        /* even n -> 0 */
    if (n & 1) {        /* odd n */
        idx_t k = (n - 1) / 6;
        t = (k & 1) ? (idx_t)-1 : (idx_t)1;
    }

    /* adjust base by t (t is 0, +1, or -1) */
    idx_t sum = base;
    if (t == (idx_t)1)
        sum += 1;
    else if (t == (idx_t)-1)
        sum -= 1;

    /* floor division by 2 */
    return (seq_t)(sum / 2);
}
