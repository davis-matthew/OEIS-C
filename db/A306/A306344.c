/*
 * A306344  The q-analogs T(q; n,k) of the rascal-triangle, here q = 3.
 * https://oeis.org/A306344
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS entry A306344:
 *  a(n) = T(3; m, k)
 *  where m = floor((sqrt(8*n + 1) – 1) / 2)
 *  and k = n – m*(m+1)/2
 *
 *  T(q; n, k) is given in the OEIS entry as
 *  1 + [(q^k – 1)/(q – 1)] * [(q^(n-k) – 1)/(q – 1)]
 *  Here q = 3, so T(3; n, k) = 1 + ((3^k – 1)/2) * ((3^(n-k) – 1)/2).
 *
 *  The sequence is obtained by listing the entries of the triangle
 *  T(3; n, k) row by row (row n, in increasing k).
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow3.h"

static idx_t find_row(idx_t n)
{
    /* Binary search for largest m such that m*(m+1)/2 <= n */
    idx_t lo = 0;
    idx_t hi = n;
    /* avoid overflow for maximum idx_t value */
    if (n != (idx_t)-1) hi = n + 1;
    while (lo + 1 < hi) {
        idx_t mid = (lo + hi) >> 1;
        unsigned __int128 v = (unsigned __int128)mid * (mid + 1) / 2;
        if (v <= n)
            lo = mid;
        else
            hi = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* locate the row m and column k of the n-th element */
    idx_t m = find_row(n);
    unsigned __int128 base128 = (unsigned __int128)m * (m + 1) / 2;
    idx_t k = (idx_t)(n - base128);               /* 0 <= k <= m */

    /* compute the q-analog T(3; m, k) = 1 + ((3^k-1)/2)*((3^(m-k)-1)/2) */
    seq_t pow_k   = pow3(k);        /* 3^k  */
    seq_t pow_mk  = pow3(m - k);    /* 3^(m-k) */

    seq_t a1 = (pow_k   - 1) / 2;   /* (3^k   - 1)/2  */
    seq_t a2 = (pow_mk  - 1) / 2;   /* (3^(m-k)- 1)/2 */

    return a1 * a2 + 1;
}
