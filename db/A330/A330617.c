/*
 * A330617  Triangle read by rows: T(n,k) is the number of paths from node 0
 *          to k in a directed graph with n+1 vertices labeled 0, 1, ..., n
 *          and edges leading from i to i+1 for all i, and from i to i+2 for
 *          even i and from i to i-2 for odd i.
 * https://oeis.org/A330617
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl, walk
 * Formula:   a(n) = T(r,k) where r = floor((sqrt(8*n+1)-1)/2) and k = n -
 *            r(r+1)/2. For the triangle entry T(r,k): if k is even: T(r,k) =
 *            2^(k/2) if k is odd : T(r,k) = 2^((k-1)/2) * (ceil(r/2) -
 *            (k-1)/2)
 * Range:     u8 n <= 151, u16 n <= 559, u32 n <= 1325, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* find the row r that contains the index n */
    idx_t r = 0;
    while ( ((__uint128_t)(r + 1) * (r + 2)) / 2 <= n )
        ++r;

    /* index inside the row */
    idx_t start = r * (r + 1) / 2;
    idx_t k = n - start;

    if (k & 1) {                /* k is odd */
        idx_t m = (k - 1) / 2;              /* m = (k-1)/2   */
        seq_t pow = pow2(m);                /* 2^m          */
        seq_t factor = (seq_t)(( (r + 1) / 2 ) - m); /* ceil(r/2)-m */
        return pow * factor;
    } else {                     /* k is even */
        return pow2(k / 2);
    }
}
