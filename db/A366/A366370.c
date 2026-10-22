/*
 * A366370  Square array A(n,k) giving the length of the least significant run
 *          of 0-bits in binary expansion of A000225(n)^k, or 0 if
 *          A000225(n)^k is a binary repunit.
 * https://oeis.org/A366370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 8282, u16 n <= 8282, u32 n <= 8282, u64 n <= 8282,
 *            u128 n <= 8282, big any n
 * Helpers:   _v2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:  For n ≥ 1, let (i,j) be the anti‑diagonal coordinates
 * of index n in the square array A(i,j).  For i=1 or j=1 the value is 0.
 * For i ≥ 2, j ≥ 2:
 *     if j is even:   A(i,j) = i + v₂(j/2)
 *     if j is odd:    A(i,j) = 1 + v₂((j−1)/2)
 * where v₂(m) is the exponent of 2 in m.
 * This reproduces the OEIS series A366370.
 */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/_v2_uint64.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Find the diagonal d such that d*(d+1)/2 ≥ n */
    idx_t d = 1;
    while (d * (d + 1) / 2 < n) ++d;

    idx_t prev = d * (d - 1) / 2;          /* elements before this diagonal */
    idx_t pos  = n - prev - 1;             /* 0‑based index within diagonal */
    idx_t i    = pos + 1;                  /* row index (≥1) */
    idx_t j    = d - pos;                  /* column index (≥1) */

    /* Zero on first row or column */
    if (i == 1 || j == 1) return 0;

    if ((j & 1ULL) == 0) {                /* even column: j = 2k */
        idx_t k = j >> 1;                 /* k = j/2 */
        seq_t v = _v2_uint64(k);
        return (seq_t)i + v;
    } else {                               /* odd column: j = 2k+1 */
        idx_t k = (j - 1) >> 1;           /* k = (j-1)/2 */
        seq_t v = _v2_uint64(k);
        return (seq_t)1 + v;
    }
}
