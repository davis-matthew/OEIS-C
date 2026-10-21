/*
 * A284855  Array read by antidiagonals: T(n,k) = number of necklaces with n
 *          beads and k colors that are the same when turned over.
 * https://oeis.org/A284855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,j) read by antidiagonals. T(2*m,k) = (k^(m+1)+k^m)/2
 *            T(2*m+1,k) = k^(m+1)
 * Range:     u8 n <= 41, u16 n <= 129, u32 n <= 368, u64 n <= 1064,
 *            u128 n <= 1275, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* compute diagonal index d such that (d-1)*d/2 >= n */
    idx_t d = 2;
    while (((__int128)(d - 1) * d) / 2 < n) ++d;

    /* previous count before diagonal d */
    __int128 prev = ((__int128)(d - 2) * (d - 1)) / 2;
    idx_t offset = (idx_t)(n - prev - 1);

    /* coordinates (i,j) in the matrix */
    idx_t i = offset + 1;            /* row index (n in T(n,k)) */
    idx_t j = (d - 1) - offset;      /* column index (k in T(n,k)) */

    /* compute T(i,j) */
    seq_t k = (seq_t)j;
    seq_t result;

    if (i & 1) {                     /* odd i: i = 2*m+1, use k^(m+1) */
        idx_t exp = (i + 1) / 2;
        result = big_pow(k, (seq_t)exp);
    } else {                         /* even i: i = 2*m, use (k^(m+1)+k^m)/2 */
        idx_t m   = i / 2;
        seq_t pow_m   = big_pow(k, (seq_t)m);
        seq_t pow_mp1 = pow_m * k;
        result = (pow_m + pow_mp1) / 2;
    }

    return result;
}
