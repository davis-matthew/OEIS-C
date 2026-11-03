/*
 * A358372  Number of nodes in the n-th standard ordered rooted tree.
 * https://oeis.org/A358372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Validated: partial, pass, 87 terms through n = 87 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; for n>1, a(n)=1+Σ_{p∈comp(n-1)} a(p)  */
/* comp(k) is the k‑th composition in standard order (graded reverse‑lexicographic). */

#include <stdint.h>
#include <stddef.h>

/* Return the composition of the given 0‑based rank.
   parts[] receives the positive parts, *len_out is the length.
   For rank 0 the composition is empty. */
static void composition_parts(idx_t rank, idx_t parts[], size_t *len_out)
{
    if (rank == 0) {
        *len_out = 0;
        return;
    }

    /* Find s such that 2^(s-1) ≤ rank < 2^s.  s is at most 64 for any
       64‑bit rank.  The loop is safe because rank < 2^63 for the
       intended input limits. */
    unsigned int s = 1;
    while (rank >= ((idx_t)1 << s))
        ++s;

    /* Local index within the block of compositions of sum s. */
    idx_t local = rank - ((idx_t)1 << (s - 1));

    size_t len = 0;
    idx_t part_len = 1;
    for (unsigned int pos = 1; pos < s; ++pos) {
        if ((local >> (s - 1 - pos)) & 1) {
            parts[len++] = part_len;
            part_len = 1;
        } else {
            ++part_len;
        }
    }
    parts[len++] = part_len;
    *len_out = len;
}

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
