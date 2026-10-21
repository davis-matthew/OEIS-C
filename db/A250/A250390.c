/*
 * A250390  Number of length 4+3 0..n arrays with no four consecutive terms
 *          having the maximum of any two terms equal to the minimum of the
 *          remaining two terms.
 * https://oeis.org/A250390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^7 + (19/30)*n^6 + (51/20)*n^5 + (17/12)*n^4 - (1/4)*n^3
 *            + (19/20)*n^2 - (3/10)*n Simplified to: a(n) = (60*n^7 + 38*n^6
 *            + 153*n^5 + 85*n^4 - 15*n^3 + 57*n^2 - 18*n) / 60 Valid for all
 *            n ≥ 1. The computation uses 128‑bit arithmetic for safety. The
 *            result is cast to seq_t, which may be uin...
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 23, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* behaviour below the offset is irrelevant */

    __int128 N = n;                  /* use signed 128‑bit type for safe intermediate */
    __int128 n2 = N * N;
    __int128 n3 = n2 * N;
    __int128 n4 = n3 * N;
    __int128 n5 = n4 * N;
    __int128 n6 = n5 * N;
    __int128 n7 = n6 * N;

    __int128 numer = 60 * n7
                    + 38 * n6
                    + 153 * n5
                    + 85 * n4
                    - 15 * n3
                    + 57 * n2
                    - 18 * N;

    return (seq_t)(numer / 60);
}
