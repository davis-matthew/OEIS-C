/*
 * A315711  Coordination sequence Gal.6.665.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315711
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = { 1, n = 0; 6*n, n > 0 and n mod 6 = 0; 6*n - floor(n/6),
 *            n mod 6 = 1 or 2; 6*n - 1 - 2*floor((n-3)/6), n mod 6 = 3; 6*n -
 *            1 - floor((n-m)/6), n mod 6 = 4 or 5 (m = n mod 6). }
 * Range:     u8 n <= 43, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t six_n = (seq_t)6 * (seq_t)n;
    idx_t m = n % 6;

    switch (m) {
    case 0:
        return six_n;
    case 1:
    case 2:
        return six_n - (seq_t)(n / 6);
    case 3:
        return six_n - (seq_t)1 - (seq_t)2 * (seq_t)((n - 3) / 6);
    case 4:
    case 5:
        return six_n - (seq_t)1 - (seq_t)((n - m) / 6);
    default:
        /* unreachable, but return six_n to satisfy compiler */
        return six_n;
    }
}
