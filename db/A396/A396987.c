/*
 * A396987  The number of n-colorings of the faces of the triakis tetrahedron
 *          up to rotation.
 * https://oeis.org/A396987
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^12 + 3*n^6 + 8*n^4)/12
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 49, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic to avoid overflow for all n
       with a(n) that fits in seq_t (i.e. n ≤ 46 for 64‑bit seq_t). */
    unsigned __int128 N = (unsigned __int128)n;

    unsigned __int128 n2 = N * N;          /* n^2 */
    unsigned __int128 n4 = n2 * n2;        /* n^4 */
    unsigned __int128 n6 = n4 * n2;        /* n^6 */
    unsigned __int128 n12 = n6 * n6;       /* n^12 */

    unsigned __int128 numerator = n12 + 3 * n6 + 8 * n4;
    unsigned __int128 quotient  = numerator / 12;

    return (seq_t)quotient;
}
