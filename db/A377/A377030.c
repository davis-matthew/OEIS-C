/*
 * A377030  Period 19: repeat [0, 3, 6, 9, 7, 4, 1, 2, 5, 8, 8, 5, 2, 1, 4, 7,
 *          9, 6, 3].
 * https://oeis.org/A377030
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = min(3*n - 19*floor(3*n/19), 19*ceil(3*n/19) - 3*n)
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 3*n mod 19 can be computed safely with idx_t arithmetic */
    idx_t r = ((n % 19u) * 3u) % 19u;          /* r = (3*n) mod 19 */
    idx_t alt = 19u - r;                       /* distance to next multiple of 19 */
    return (seq_t)(r < alt ? r : alt);         /* return the smaller distance */
}
