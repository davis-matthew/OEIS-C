/*
 * A247853  The 8th Hermite Polynomial evaluated at n: H_8(n) =
 *          256*n^8-3584*n^6+13440*n^4-13440*n^2+1680.
 * https://oeis.org/A247853
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 256*n^8 - 3584*n^6 + 13440*n^4 - 13440*n^2 + 1680
 * Range:     i8 none, i16 n <= 2, i32 n <= 7, i64 n <= 117, i128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* use 128‑bit integers for all intermediate arithmetic */
    __int128 x  = (__int128)n;
    __int128 x2 = x * x;
    __int128 x4 = x2 * x2;
    __int128 x6 = x4 * x2;
    __int128 x8 = x4 * x4;
    __int128 res = 256 * x8 - 3584 * x6 + 13440 * x4 - 13440 * x2 + 1680;
    return (seq_t)res;
}
