/*
 * A243215  Number of 4-matchings of the n X n grid graph.
 * https://oeis.org/A243215
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n^8 -16*n^7 -60*n^6 +308*n^5 +171*n^4 -1942*n^3
 *            +872*n^2 +3963*n -3366)/6 for n>=4 a(3) = 18 a(n) = 0 for n<=2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 17, u64 n <= 269, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* small cases */
    if (n <= 2)
        return 0;
    if (n == 3)
        return 18;

    /* use 128‑bit intermediate to avoid overflow of the degree‑8
       polynomial for values that still fit into seq_t.  The maximum
       n used in the test suite (≈10000) gives 64‑bit results; the
       intermediate value fits into 128 bits. */
    __int128 N  = (unsigned long long)n;
    __int128 N2 = N * N;
    __int128 N3 = N2 * N;
    __int128 N4 = N3 * N;
    __int128 N5 = N4 * N;
    __int128 N6 = N5 * N;
    __int128 N7 = N6 * N;
    __int128 N8 = N7 * N;

    __int128 num =  4 * N8
                  - 16 * N7
                  - 60 * N6
                  + 308 * N5
                  + 171 * N4
                  - 1942 * N3
                  + 872 * N2
                  + 3963 * N
                  - 3366;

    num /= 6;

    return (seq_t)num;
}
