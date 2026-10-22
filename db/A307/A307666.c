/*
 * A307666  Number of partitions of n into consecutive positive triangular
 *          numbers.
 * https://oeis.org/A307666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 4943, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   max_j
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function counts the number of partitions of n into
   consecutive positive triangular numbers by checking all pairs
   of tetrahedral numbers whose difference equals n.  The
   algorithm runs in O(sqrt(n) log n) time. */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/max_j.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* seq starts from a(1) */
    idx_t jmax = max_j(n);  /* largest j with triangular(j) <= n */
    if (jmax == 0) return 0;

    /* Allocate array for tetrahedral numbers up to jmax */
    size_t cap = jmax + 1;
    seq_t *tet = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!tet) return 0;

    tet[0] = 0;               /* T_0 = 0 */
    for (idx_t k = 1; k <= jmax; ++k) {
        __int128 tri = (__int128)k * (k + 1) / 2;
        tet[k] = tet[k - 1] + (seq_t)tri;
    }

    seq_t count = 0;
    for (idx_t j = 1; j <= jmax; ++j) {
        if (tet[j] < n) continue;          /* difference negative */
        seq_t d = tet[j] - (seq_t)n;        /* T_j - n */
        /* binary search d in tet[0 .. j-1] */
        size_t lo = 0, hi = j - 1, mid, found = 0;
        while (lo <= hi) {
            mid = lo + (hi - lo) / 2;
            if (tet[mid] == d) {
                found = 1;
                break;
            } else if (tet[mid] < d) {
                lo = mid + 1;
            } else {
                if (mid == 0) break;
                hi = mid - 1;
            }
        }
        if (found) count++;
    }
    free(tet);
    return count;
}
