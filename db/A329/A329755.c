/*
 * A329755  Doubly hexagonal pyramidal numbers.
 * https://oeis.org/A329755
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(4*n-1)*(n+1)*(4*n^3+3*n^2-n+6)*(8*n^3+6*n^2-2*n-3)/648
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 13, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use a 128‑bit temporary so intermediate products never overflow.
       The final result always fits in seq_t (the build type). */
    unsigned __int128 nn = n;
    unsigned __int128 res = nn;

    res *= (4*nn - 1);      /* n * (4n-1)   */
    res *= (nn + 1);        /* * (n+1)      */

    unsigned __int128 p1 = 4*nn*nn*nn + 3*nn*nn - nn + 6;   /* 4n³+3n²-n+6 */
    unsigned __int128 p2 = 8*nn*nn*nn + 6*nn*nn - 2*nn - 3;   /* 8n³+6n²-2n-3 */

    res *= p1;             /* * (4n³+3n²-n+6) */
    res *= p2;             /* * (8n³+6n²-2n-3) */
    res /= (unsigned __int128)648;  /* divide by 648 */

    return (seq_t)res;
}
