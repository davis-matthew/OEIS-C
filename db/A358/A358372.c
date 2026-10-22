/*
 * A358372  Number of nodes in the n-th standard ordered rooted tree.
 * https://oeis.org/A358372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   composition_parts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; for n>1, a(n)=1+Σ_{p∈comp(n-1)} a(p)  */
/* comp(k) is the k‑th composition in standard order (graded reverse‑lexicographic). */

#include <stdint.h>
#include <stddef.h>
#include "../utils/composition_parts.h"

/* Main sequence computation.  Recursively evaluates a(n). */
seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    idx_t r = n - 1;
    idx_t parts[65];
    size_t len;
    composition_parts(r, parts, &len);

    seq_t sum = 1;                     /* count the root node */
    for (size_t i = 0; i < len; ++i)
        sum += a(parts[i]);             /* recurse on each part */

    return sum;
}
