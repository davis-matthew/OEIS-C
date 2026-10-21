/*
 * A260513  a(n) = (8*n+13*n^3+3*n^5)/24; also the sum of triangular numbers
 *          taken in successive groups of increasing size (see Example).
 * https://oeis.org/A260513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n + 13*n^3 + 3*n^5)/24
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 127, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Ensure we are not dividing by zero when n is 0 (not part of the sequence) */
    if (n == 0)
        return 0;

    /* Compute using 128‑bit arithmetic to avoid overflow for large n */
    unsigned __int128 N   = n;
    unsigned __int128 n2  = N * N;           /* n^2 */
    unsigned __int128 n4  = n2 * n2;         /* n^4 */
    unsigned __int128 term = N * ( (unsigned __int128)8
                                   + (unsigned __int128)13 * n2
                                   + (unsigned __int128)3  * n4 );
    term /= 24;

    return (seq_t)term;
}
