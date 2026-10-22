/*
 * A378682  Triangle T(n,k) read by rows: where T(n,k) is the number of the
 *          k-th eliminated person in the variation of the Josephus
 *          elimination process for n people, where the first person is
 *          eliminated, and after that, every second person is eliminated.
 * https://oeis.org/A378682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Triangle T(row, position) where T(row,1)=1 and T(row,k)
 *            is the k‑th eliminated person in Josephus with step 2 starting
 *            with removal of person 1. The sequence is produced by flattening
 *            the rows: 1; 1,2; 1,3,2; 1,3,5,4,2;
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   kth_eliminated
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include "../utils/kth_eliminated.h"

/* Helper: integer square root of an unsigned __int128 value.
   Returns floor(sqrt(x)). */
static idx_t isqrt_u128(unsigned __int128 x)
{
    long double d = sqrt((long double)x);
    idx_t r = (idx_t)d;
    /* Adjust to guarantee floor */
    while ((unsigned __int128)(r + 1) * (r + 1) <= x) r++;
    while ((unsigned __int128)r * r > x) r--;
    return r;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 >= n */
    unsigned __int128 val = (unsigned __int128)8 * n + 1;
    idx_t sq = isqrt_u128(val);
    idx_t r = (sq - 1) / 2;

    /* Correct r if needed */
    while ((unsigned __int128)r * (r + 1) / 2 < n) r++;
    while (r > 0 && (unsigned __int128)(r - 1) * r / 2 >= n) r--;

    /* Position k within the row */
    idx_t k = n - (unsigned __int128)(r - 1) * r / 2;

    /* Compute the k-th eliminated person in row r */
    return kth_eliminated(r, k);
}
