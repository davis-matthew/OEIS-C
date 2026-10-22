/*
 * A343558  Irregular triangle read by rows: the n-th row gives the row
 *          indices of the consecutive elements of the spiral of the n X n
 *          matrix defined in A126224.
 * https://oeis.org/A343558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, tabf
 * Formula:   not stated
 * Range:     u8 n <= 9455, u16 n <= 9455, u32 n <= 9455, u64 n <= 9455,
 *            u128 n <= 9455, big any n
 * Helpers:   get_row_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_row_index.h"
/* Computes a(n) as the row index of the n-th element in the
   concatenated rows of spirals of size 1×1, 2×2, 3×3, …,
   i.e. the irregular triangle read by rows (OEIS A343558). */
 

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* safety, not used for official offset 1 */

    /* Find the minimal k such that the cumulative number of cells
       up to size k is ≥ n.  The cumulative is sum_{i=1}^{k-1} i^2.
       Incrementally add squares to avoid overflow. */
    idx_t cumulative = 0;    /* sum of squares up to k-1 */
    idx_t k = 0;

    while (1) {
        idx_t k_next = k + 1;
        idx_t sq = k_next * k_next;
        if (cumulative + sq >= n) {
            k = k_next;
            break;
        }
        cumulative += sq;
        k = k_next;
    }

    idx_t pos = n - cumulative;   /* 1‑based position within the k×k spiral */
    idx_t row = get_row_index(k, pos);
    return (seq_t)row;
}
