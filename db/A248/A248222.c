/*
 * A248222  Maximal gap between quadratic residues mod n.
 * https://oeis.org/A248222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = maximum difference between consecutive quadratic residues
 *            modulo n, with wrap‑around
 * Range:     u8 n <= 18, u16 n <= 985, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* n must be at least 1; for n == 0 we return 0 as a safe fallback */
    if (n == 0) return 0;

    /* Allocate a boolean array to mark residues.  Use calloc for zero‑initialisation. */
    uint8_t *mark = (uint8_t *)calloc((size_t)n, 1);
    if (!mark) return 0;  /* allocation failed – unable to compute */

    /* Mark each residue r = i^2 mod n for i = 0 .. n‑1. */
    for (idx_t i = 0; i < n; ++i) {
        seq_t sq = (seq_t)i * (seq_t)i;
        seq_t r  = sq % (seq_t)n;
        mark[(size_t)r] = 1;
    }

    /* Find the maximum gap between successive marked residues, treating the list cyclically. */
    idx_t first = 0, prev = 0;
    bool  first_found = false;
    seq_t maxgap = 0;

    for (size_t r = 0; r < n; ++r) {
        if (mark[(size_t)r]) {
            if (!first_found) {
                first = (idx_t)r;
                first_found = true;
            } else {
                seq_t gap = (seq_t)r - (seq_t)prev;
                if (gap > maxgap) maxgap = gap;
            }
            prev = (idx_t)r;
        }
    }

    /* Wrap‑around gap from the last residue back to the first (plus n). */
    seq_t wrapgap = ((seq_t)n - (seq_t)prev) + (seq_t)first;
    if (wrapgap > maxgap) maxgap = wrapgap;

    free(mark);
    return maxgap;
}
