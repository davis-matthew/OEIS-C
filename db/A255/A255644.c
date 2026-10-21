/*
 * A255644  Difference between sums of quadratic residues and non-residues
 *          modulo n (residues are not necessarily coprime to n).
 * https://oeis.org/A255644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{k∈QRes(n)} k – Σ_{k∉QRes(n)} k where QRes(n) = { k |
 *            ∃x: x² ≡ k (mod n) }. Equivalent to a(n) = 2·Σ_{k∈QRes(n)} k –
 *            n·(n‑1)/2
 * Range:     i8 n <= 23, i16 n <= 100, i32 n <= 100, i64 n <= 100,
 *            i128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* n = 0 or 1 falls outside the OEIS offset – return 0. */
    if (n <= 1)
        return 0;

    /* Bitmap of residues found so far.  One byte per possible residue. */
    char *seen = (char *)calloc((size_t)n, 1);
    if (!seen)
        return 0;            /* Out‑of‑memory: safe fallback. */

    seq_t sum_residues = 0;

    for (idx_t i = 0; i < n; ++i) {
        /* Compute i² mod n, staying in seq_t to avoid overflow for
           arbitrary‑precision builds.  The result is guaranteed to be < n,
           so we can safely cast to idx_t for the bitmap lookup. */
        seq_t sq_val = ((seq_t)i * (seq_t)i) % (seq_t)n;
        idx_t sq = (idx_t)sq_val;
        if (!seen[sq]) {
            seen[sq] = 1;
            sum_residues += (seq_t)sq;
        }
    }

    free(seen);

    /* total = Σ_{k=0}^{n-1} k = n·(n‑1)/2 */
    seq_t total = ((seq_t)n * ((seq_t)n - 1)) / 2;

    /* Result = 2·sum_residues – total. */
    return sum_residues * 2 - total;
}
