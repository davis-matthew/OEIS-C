/*
 * A374338  Start with two vertices and draw a circle around each whose radius
 *          is the distance between the vertices. The sequence gives the
 *          number of vertices constructed after n iterations of drawing
 *          circles with this same radius around every new vertex created from
 *          all circles' intersections. See the Comme...
 * https://oeis.org/A374338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = { * (3*n^2+5*n+4)/3 if n mod 3 == 1, * (3*n^2+5*n) /3 if
 *            n mod 3 == 0, * (3*n^2+5*n+2)/3 if n mod 3 == 2. * }
 * Range:     u8 n <= 15, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nm = (seq_t)n;           /* ensure all arithmetic is in seq_t */
    seq_t r;

    switch ((int)(n % 3)) {
    case 1:                       /* n ≡ 1 (mod 3) */
        r = (3 * nm * nm + 5 * nm + 4) / 3;
        break;
    case 0:                       /* n ≡ 0 (mod 3) */
        r = (3 * nm * nm + 5 * nm) / 3;
        break;
    default:                      /* n ≡ 2 (mod 3) */
        r = (3 * nm * nm + 5 * nm + 2) / 3;
        break;
    }
    return r;
}
