/*
 * A268947  Number of length-6 0..n arrays with no repeated value unequal to
 *          the previous repeated value plus one mod n+1.
 * https://oeis.org/A268947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^6 + 6*n^5 + 5*n^4 + 6*n^3 + 3*n^2 - n + 2
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 39, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (OEIS A268947, offset 1) */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle offset: a(1) is first term, but formula works for n>=1.
       For n == 0 return 0 (undefined case). */
    if (n == 0) return 0;

    /* Compute using 128‑bit intermediate to avoid overflow for n up to 10000. */
    __int128 tn   = (__int128)n;       /* n as 128‑bit integer   */
    __int128 tn2  = tn * tn;           /* n^2 */
    __int128 tn3  = tn2 * tn;          /* n^3 */
    __int128 tn4  = tn3 * tn;          /* n^4 */
    __int128 tn5  = tn4 * tn;          /* n^5 */
    __int128 tn6  = tn5 * tn;          /* n^6 */

    __int128 sum =
        tn6              /* n^6      */
        + 6 * tn5        /* 6*n^5    */
        + 5 * tn4        /* 5*n^4    */
        + 6 * tn3        /* 6*n^3    */
        + 3 * tn2        /* 3*n^2    */
        - tn             /* -n       */
        + 2;             /* +2       */

    return (seq_t)sum;
}
