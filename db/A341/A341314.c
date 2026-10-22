/*
 * A341314  Array read by antidiagonals: T(n,k) = (n+k)/gcd(n,k), n>=0, k>=0.
 * https://oeis.org/A341314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n-th element obtained by reading the array * T(i,j)
 *            = (i+j)/gcd(i,j) (with T(0,0)=0) by antidiagonals with * offsets
 *            i=0..d, j=d-i for each d=0,1,2,
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Find diagonal d such that d*(d+1)/2 <= n < (d+1)*(d+2)/2. */
    idx_t d = (idx_t)((sqrt((long double)1 + 8.0L * (long double)n) - 1.0L) / 2.0L);
    /* Adjust in case of rounding errors. */
    unsigned __int128 start = (unsigned __int128)d * (d + 1) / 2;
    while (start > n) {
        d--;
        start = (unsigned __int128)d * (d + 1) / 2;
    }
    unsigned __int128 next_start = (unsigned __int128)(d + 1) * (d + 2) / 2;
    while (next_start <= n) {
        d++;
        next_start = (unsigned __int128)(d + 1) * (d + 2) / 2;
    }

    /* Position inside diagonal d */
    idx_t offset = n - (idx_t)start;
    idx_t i = offset;          /* row   (x-coordinate) */
    idx_t j = d - offset;      /* column (y-coordinate) */

    if (i == 0 && j == 0)
        return 0;              /* special case defined by OEIS */

    seq_t sum = (seq_t)i + (seq_t)j;
    seq_t g   = gcd((seq_t)i, (seq_t)j);
    return sum / g;
}
