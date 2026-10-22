/*
 * A395698  Triangle read by rows: row n (n >= 1) gives the concatenated
 *          run-length encoding of the integer partitions of n, sorted first
 *          by length and then reverse-lexicographically.
 * https://oeis.org/A395698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   gen_len_partitions
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by iteratively generating the triangle rows of
   run‑length encoded integer partitions, concatenating the
   resulting integers, and returning the n‑th element in the
   flattened sequence.  */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/gen_len_partitions.h"

/* Public entry point required by the build system.  */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safety, though n ≥ 1 for the sequence */

    idx_t target = n;
    idx_t count  = 0;
    seq_t result = 0;

    for (idx_t r = 1; ; ++r) {
        /* Allocate array for the largest possible partition of r. */
        size_t len = (size_t)r;
        int *parts = (int *)malloc(len * sizeof(int));
        if (!parts) return 0;             /* allocation failure */

        /* Generate partitions grouped by increasing length (#parts). */
        for (int k = 1; k <= (int)r; ++k) {
            if (gen_len_partitions((idx_t)r, (int)r, k, parts, 0,
                                   &count, target, &result)) {
                free(parts);
                return result;
            }
        }
        free(parts);
    }
}
