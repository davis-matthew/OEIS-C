/*
 * A315656  Coordination sequence Gal.6.667.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315656
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   let m = n mod 6 let k = floor(n/6) r(n) = 0 if m == 0 2*k if m
 *            == 1 or 2 4*k + 2 if m == 3 2*k + 2 if m == 4 or 5 a(n) = 6*n -
 *            r(n) for n >= 1 a(0) = 1
 * Range:     u8 n <= 45, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    idx_t m = n % 6;
    seq_t k = n / 6;          /* floor(n/6) as seq_t */
    seq_t r;

    switch (m) {
        case 0:                 /* m == 0 */
            r = 0;
            break;
        case 1:                 /* m == 1 */
        case 2:                 /* m == 2 */
            r = 2 * k;
            break;
        case 3:                 /* m == 3 */
            r = 4 * k + 2;      /* 4*floor(n/6)+2 */
            break;
        case 4:                 /* m == 4 */
        case 5:                 /* m == 5 */
            r = 2 * k + 2;      /* 2*floor(n/6)+2 */
            break;
        default:
            r = 0;              /* unreachable */
            break;
    }

    seq_t nn = (seq_t)n;
    return 6 * nn - r;
}
