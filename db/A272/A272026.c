/*
 * A272026  Triangle read by rows: T(n,k), n>=1, k>=1, in which column k lists
 *          the numbers A016945 interleaved with k-1 zeros, and the first
 *          element of column k is in row k(k+1)/2.
 * https://oeis.org/A272026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the element at position n in the triangle T(n,k) where 1
 *            ≤ k ≤ len(n), len(n)=⌊(√(8n+1)−1)/2⌋ and T(n,k) = A016945(
 *            (n−k(k+1)/2)/k + 1 ) if (n−k(k+1)/2)≡0 (mod k), otherwise 0.
 *            A016945(m) = 3·(2m−1). This implements the definition given in
 *            A272026. The function works for all n≥1 and for a...
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   len_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* include standard headers that may be required */
#include <stddef.h>
#include <stdint.h>
#include <math.h>
#include "../utils/len_row.h"

seq_t a(idx_t n)
{
    /* find the row r that contains the n-th term in the flattened triangle */
    idx_t r = 1, cumulative = 0;
    while (1) {
        idx_t len = len_row(r);
        if (cumulative + len >= n) break;
        cumulative += len;
        ++r;
    }
    /* column index within the row (1‑based) */
    idx_t k = n - cumulative;
    /* compute the value using the definition */
    idx_t first_row = (k * (k + 1)) / 2;
    if (r < first_row) {
        /* column k has not started yet – should not occur but guard anyway */
        return 0;
    }
    idx_t offset = r - first_row;
    if (offset % k != 0) return 0;
    idx_t t = offset / k + 1;          /* index within A016945 */
    seq_t val = (seq_t)3 * (seq_t)(2 * t - 1);  /* A016945(t) */
    return val;
}
