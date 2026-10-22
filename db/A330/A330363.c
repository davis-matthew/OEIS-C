/*
 * A330363  58 consecutive function values of the prime generating polynomial
 *          P(x) = (1/72)*x^6 + (1/24)*x^5 - (1583/72)*x^4 - (3161/24)*x^3 +
 *          (200807/36)*x^2 + (97973/3)*x - 11351: abs(P(n)) is prime for -45
 *          <= n <= 12.
 * https://oeis.org/A330363
 *
 * Offset:    -45    (a(-45) is the first term)
 * Keywords:  sign, fini, full, less
 * Formula:   a(n) = (1/72)*(n^6 + 3*n^5 - 1583*n^4 - 9483*n^3 + 401614*n^2 +
 *            2351352*n - 817272)
 * Range:     i8 none, i16 none, i32 n <= 12, i64 n <= 12, i128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use a wider type for intermediate calculations to avoid overflow
       for all n up to at least 10,000, which is far beyond the sequence
       offset. */
    __int128 t  = n;           /* cast to 128-bit */
    __int128 t2 = t * t;       /* n^2 */
    __int128 t3 = t2 * t;      /* n^3 */
    __int128 t4 = t3 * t;      /* n^4 */
    __int128 t5 = t4 * t;      /* n^5 */
    __int128 t6 = t5 * t;      /* n^6 */

    __int128 val = t6
                 + ( (__int128)3 ) * t5
                 - ( (__int128)1583 ) * t4
                 - ( (__int128)9483 ) * t3
                 + ( (__int128)401614 ) * t2
                 + ( (__int128)2351352 ) * t
                 - ( (__int128)817272 );

    val /= (__int128)72;  /* exact division */

    return (seq_t)val;
}
