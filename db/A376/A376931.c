/*
 * A376931  Number of points out of n points placed on a circle that can never
 *          be reached when starting at one point and then moving clockwise in
 *          incrementally increasing steps (first 1, then 2, 3, 4, ...).
 * https://oeis.org/A376931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - (number of distinct residues of k(k+1)/2 modulo n for
 *            0 ≤ k < n)
 * Range:     u8 n <= 23, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* safeguard, not used in OEIS offset */
    /* Allocate a flag array for residues 0..n-1 */
    bool *seen = (bool*)malloc((size_t)n * sizeof(bool));
    if (!seen) return 0;                 /* memory allocation failure */
    /* initialize flags to false */
    for (idx_t i = 0; i < n; ++i) seen[i] = false;

    seq_t tri = 0;                        /* triangular number accumulator */
    seq_t visited = 0;                    /* count of distinct residues */

    for (idx_t k = 0; k < n; ++k) {
        seq_t idx = tri % n;              /* residue for current k */
        idx_t pos = (idx_t)idx;           /* index into boolean array */
        if (!seen[pos]) {
            seen[pos] = true;
            ++visited;
        }
        tri += (seq_t)(k + 1);            /* next triangular number */
    }

    free(seen);
    return (seq_t)(n - visited);
}
