/*
 * A359404  Number of unordered triples of self-avoiding paths with nodes that
 *          cover all vertices of a convex n-gon.
 * https://oeis.org/A359404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*(n-2)*2^(n-10)*(3^(n-4) - 2^(n-3) + 1) for n > 3,
 *            a(n) = 0 for n ≤ 3
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 25, u128 n <= 49,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n <= 3)
        return 0;

    seq_t sn = (seq_t)n;
    seq_t A = sn * (sn - 1) * (sn - 2);

    seq_t pow3_val = pow3(n - 4);          /* 3^(n-4) */
    seq_t pow2b_val = pow2(n - 3);         /* 2^(n-3) */
    seq_t B = pow3_val - pow2b_val + (seq_t)1; /* (3^(n-4) - 2^(n-3) + 1) */

    if (n >= 10) {
        seq_t pow2a_val = pow2(n - 10);    /* 2^(n-10) */
        return A * B * pow2a_val;
    } else {
        seq_t denom = pow2(10 - n);        /* 2^(10-n) */
        return (A * B) / denom;
    }
}
