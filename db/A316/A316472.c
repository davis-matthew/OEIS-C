/*
 * A316472  Inverse permutation to A316385.
 * https://oeis.org/A316472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (OEIS A316472):
 *   a(n) = (2*b(n)+1) * 2^(L(n) - L(b(n)) - 1)
 *   where b(n) = A053645(A153142(n)),
 *   L(n) = floor(log2(n)) for n > 0 with L(0) = -1.
 * This implementation evaluates the above directly. */

#include <stdbool.h>
#include "../utils/pow_uint.h"

/* floor(log2(n)) for n > 0  (idx_t is uint64_t) */
static inline unsigned floor_log2_uint64(idx_t n)
{
    unsigned r = 0;
    while (n >>= 1) ++r;
    return r;
}

/* Compute b(n) as described in the OEIS entry:
 *  b(1) = 0
 *  For n > 1:
 *     let k = floor_log2(n), half = 2^(k-1), block_start = 2^k
 *     if n < block_start + half   → b(n) = n - half
 *     else                        → b(n) = b(n - block_start)
 */
static idx_t compute_b(idx_t n)
{
    while (true) {
        if (n == (idx_t)1)
            return (idx_t)0;

        idx_t k = (idx_t)floor_log2_uint64(n);
        idx_t half = ((idx_t)1) << (k - 1);           /* 2^(k-1)   */
        idx_t block_start = ((idx_t)1) << k;          /* 2^k       */

        if (n < block_start + half)                  /* first half */
            return n - half;
        else                                         /* second half */
            n -= block_start;                        /* recurse on smaller n */
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)            /* offset is 1; 0 is undefined, return 0 */
        return (seq_t)0;

    idx_t b = compute_b(n);
    int Ln = (int)floor_log2_uint64(n);
    int Lb = (b == (idx_t)0) ? -1
                            : (int)floor_log2_uint64(b);
    idx_t e = (idx_t)(Ln - Lb - 1);          /* exponent (always non‑negative) */

    seq_t odd = (seq_t)(2 * b + 1);           /* always odd */
    seq_t pow2 = pow_uint((uint64_t)2, e);    /* 2^e in seq_t */

    return odd * pow2;
}
