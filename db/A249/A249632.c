/*
 * A249632  Triangular array read by rows. T(n,k) is the number of labeled
 *          trees with black and white nodes having exactly k black nodes,
 *          n>=0, 0<=k<=n.
 * https://oeis.org/A249632
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where * r = floor((sqrt(8*n+1)-1)/2), * k = n -
 *            r*(r+1)/2, * and T(r,k) = r^(r-2) * binom(r,k) with r^(r-2)=1
 *            for r≤1
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   binom, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n) {
    /* Find the row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2. */
    idx_t r = 0;
    while ( (__int128)(r + 1) * (r + 2) / 2 <= n ) {
        ++r;
    }

    /* k is the column index within the row */
    idx_t prev = (idx_t)((__int128)r * (r + 1) / 2);
    idx_t k = n - prev;

    /* Base cases: r = 0 or 1 gives T(r,k) = 1. */
    if (r <= 1) {
        return (seq_t)1;
    }

    /* Compute r^(r-2) and multiply by binomial coefficient. */
    seq_t power = pow_uint((uint64_t)r, r - 2);
    seq_t bin   = binom(r, k);
    return power * bin;
}
