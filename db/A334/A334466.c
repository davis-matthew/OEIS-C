/*
 * A334466  Square array read by antidiagonals upwards: T(n,k) is the total
 *          number of parts in all partitions of n into consecutive parts that
 *          differ by k, with n >= 1, k >= 0.
 * https://oeis.org/A334466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   T_of_nk
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A334466: a(n) = T(row, col) where (row, col) is obtained
 * by reading the square array T(n,k) by antidiagonals upwards.
 * Formula for a(n):
 *   Let t = ceil( ( -1 + sqrt(1 + 8*n) ) / 2 )
 *   (row, col) = (t - r, r) with r = n - (t-1)*t/2 - 1
 *   a(n) = T(row, col)
 *
 * T(n,k) is computed with the closed form:
 *   T(n,k) = sum_{L>=1,  L + k*L*(L-1)/2 <= n, 
 *                (n - k*L*(L-1)/2) mod L == 0 } L
 * This counts all partitions of n into consecutive parts differing by k
 * and sums the number of parts in each partition.
 *
 * The implementation uses only integer arithmetic.
 */
#include <stdint.h>
#include <stddef.h>
#include "../utils/T_of_nk.h"

/* integer square root of an unsigned 128-bit number */
static uint64_t isqrt128(unsigned __int128 x)
{
    unsigned __int128 low = 0, high = ((unsigned __int128)1 << 64) - 1;
    while (low <= high) {
        unsigned __int128 mid = (low + high) >> 1;
        unsigned __int128 sq = mid * mid;
        if (sq == x) return (uint64_t)mid;
        if (sq < x) low = mid + 1;
        else        high = mid - 1;
    }
    return (uint64_t)high;      /* floor(sqrt) */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, but guard against zero */

    /* compute t = ceil((sqrt(1+8*n)-1)/2) */
    unsigned __int128 inner = (unsigned __int128)1 + (unsigned __int128)8 * n;
    uint64_t root = isqrt128(inner);          /* floor(sqrt(1+8n)) */
    uint64_t t = (root - 1) / 2;              /* floor part */
    /* adjust if not enough */
    while ((unsigned __int128)t * (t + 1) / 2 < n) ++t;
    uint64_t s = t - 1;                       /* diagonal index */

    /* D(s-1) = s*(s+1)/2 */
    unsigned __int128 prev = (unsigned __int128)s * (s + 1) / 2;
    uint64_t r = (uint64_t)(n - prev - 1);    /* offset within diagonal */
    uint64_t row = s + 1 - r;                 /* n in T(n,k) */
    uint64_t col = r;                         /* k in T(n,k) */

    return T_of_nk(row, col);
}
