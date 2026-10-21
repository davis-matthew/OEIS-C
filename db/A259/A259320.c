/*
 * A259320  a(n) = 2*n*A259319(n) - A259110(n)^2.
 * https://oeis.org/A259320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (64*(n^2-5*n^4+4*n^6))/45
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 30, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Return 0 for n == 0 (outside the offset). */
    if (n == 0) return 0;

    /* Use 128‑bit intermediate to avoid overflow up to at least
       n ≈ 10 000 (the typical limit for the tests). */
    __int128 nn = n;               /* n          */
    __int128 n2 = nn * nn;         /* n²         */
    __int128 n4 = n2 * n2;         /* n⁴         */
    __int128 n6 = n4 * n2;         /* n⁶         */
    __int128 p  = 4 * n6 - 5 * n4 + n2;   /* n²-5n⁴+4n⁶ */
    __int128 r  = ( (__int128)64 * p ) / 45;   /* final result */

    return (seq_t)r;
}
