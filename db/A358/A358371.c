/*
 * A358371  Number of leaves in the n-th standard ordered rooted tree.
 * https://oeis.org/A358371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of leaves in the n-th standard ordered rooted tree
 *            computed by recursively interpreting the (n-1)-th composition in
 *            standard ordered composition sequence (graded
 *            reverse‑lexicographic) as the root parts and summing the leaves
 *            of the subtrees indexed by those parts. The implementatio...
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   leaf_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/leaf_value.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* undefined for offset 0, but keep safe */
    return leaf_value(n);
}
