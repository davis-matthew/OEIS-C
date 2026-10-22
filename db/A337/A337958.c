/*
 * A337958  Number of achiral colorings of the 8 cubic facets of a tesseract
 *          or of the 8 vertices of a hyperoctahedron.
 * https://oeis.org/A337958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 * (n+1)^2 * (n+3) * (n^2 - 2*n + 4) / 48
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                       /* safe guard for index < 1 */
    unsigned __int128 N = n;
    unsigned __int128 D = N * N - 2 * N + 4;     /* n^2 - 2*n + 4 */
    unsigned __int128 t = N * N;                /* n^2 */
    t *= (N + 1);                               /* * (n+1) */
    t *= (N + 1);                               /* * (n+1) again */
    t *= (N + 3);                               /* * (n+3) */
    t *= D;                                     /* * D */
    t /= 48;                                     /* divide by 48 */
    return (seq_t)t;                             /* cast back to seq_t */
}
