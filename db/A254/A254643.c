/*
 * A254643  Third partial sums of ninth powers (A001017).
 * https://oeis.org/A254643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n+2)*(n+3)*(-50 +84*n +127*n^2 -204*n^3 -97*n^4
 *            +126*n^5 +98*n^6 +24*n^7 +2*n^8)/2640 (OEIS A254643)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 10, u64 n <= 71, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* undefined for n=0, harmless */

    /* use a signed 128‑bit integer for intermediate calculations;
       this keeps all intermediate products exact for all n up to at
       least 10000, which is more than enough for 64‑bit builds. */
    __int128 nn   = n;
    __int128 n1   = nn;              /* n */
    __int128 n2   = nn + 1;          /* n+1 */
    __int128 n3   = nn + 2;          /* n+2 */
    __int128 n4   = nn + 3;          /* n+3 */

    /* product n*(n+1)*(n+2)*(n+3) */
    __int128 prod = n1 * n2 * n3 * n4;

    /* polynomial in n */
    __int128 poly = -(__int128)50
                    + (__int128)84  * n1
                    + (__int128)127 * n1 * n1
                    - (__int128)204 * n1 * n1 * n1
                    - (__int128)97  * n1 * n1 * n1 * n1
                    + (__int128)126 * n1 * n1 * n1 * n1 * n1
                    + (__int128)98  * n1 * n1 * n1 * n1 * n1 * n1
                    + (__int128)24  * n1 * n1 * n1 * n1 * n1 * n1 * n1
                    + (__int128)2   * n1 * n1 * n1 * n1 * n1 * n1 * n1 * n1;

    /* final value, integer division */
    __int128 val = prod * poly / 2640;

    return (seq_t)val;
}
