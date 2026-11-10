/*
 * A370791  Inverse permutation of A370790.
 * https://oeis.org/A370791
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = inverse permutation of A370790; a(0)=0. For n>=1 with
 *            k=floor(log2 n), a(n) = 2^k + o, where o in [0,2^k) is the
 *            unique offset with f(k,o) = n - 2^k (f as in A370790), found by
 *            undoing the level-by-level bit decisions
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 84 terms through n = 9641 (2026-11-10)
 * Helpers:   ilog2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/ilog2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    unsigned k = ilog2(n);
    idx_t p = ((idx_t)1) << k;
    idx_t t = n - p;
    idx_t m = 0;
    unsigned j;
    for (j = 1; j <= k; j++) {
        idx_t half = ((idx_t)1) << (j - 1);
        if ((t >> (j - 1)) & 1) {
            m = half + (m + 1) % half;
        }
    }
    return (seq_t)(p + m);
}
