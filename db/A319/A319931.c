/*
 * A319931  a(n) = -(1/120)*n*(n - 3)*(n - 6)*(n^2 - 21*n + 8).
 * https://oeis.org/A319931
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -(1/120)*n*(n - 3)*(n - 6)*(n^2 - 21*n + 8)
 * Range:     i8 n <= 8, i16 n <= 28, i32 n <= 197, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Offset: 0  (a(0) is the first term) */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic to avoid intermediate overflow.  */
    __int128 nn   = n;                         /* cast index to 128‑bit */
    __int128 term1 = nn;                       /* n          */
    __int128 term2 = nn - 3;                   /* n-3        */
    __int128 term3 = nn - 6;                   /* n-6        */
    __int128 term4 = nn * nn - (__int128)21 * nn + 8;  /* n^2-21n+8 */
    __int128 prod  = term1 * term2 * term3 * term4;   /* product     */
    prod = -prod;                              /* negative    */
    prod /= (__int128)120;                      /* divide by 120 */
    return (seq_t)prod;                         /* cast to result type */
}
