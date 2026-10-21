/*
 * A279111  Number of non-equivalent ways to place 2 non-attacking kings on an
 *          n X n board.
 * https://oeis.org/A279111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^4 - 2*n^2 - 4*n + IF(n odd, 2*n^2 - 4*n + 7, 0)) / 16
 *            Simplified: if (n is odd) a(n) = (n^4 - 8*n + 7) / 16 if (n is
 *            even) a(n) = (n^4 - 2*n^2 - 4*n) / 16 This implementation uses
 *            128‑bit arithmetic for safety and casts to seq_t only for the
 *            final result
 * Range:     u8 n <= 8, u16 n <= 32, u32 n <= 512, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The OEIS offset is 1; for n < 1 we simply return 0. */
    if (n < 1) return 0;

    /* Use signed __int128 for intermediate calculations to avoid overflow
       or underflow for small n. */
    __int128 nn = ( __int128 ) n;
    __int128 n2 = nn * nn;
    __int128 n4 = n2 * n2;
    __int128 numer;

    if (n & 1) {           /* n is odd */
        numer = n4 - 8 * nn + 7;
    } else {               /* n is even */
        numer = n4 - 2 * n2 - 4 * nn;
    }

    return (seq_t)( numer / 16 );
}
