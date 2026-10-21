/*
 * A243182  Numbers of the form 2x^2+2xy+5y^2.
 * https://oeis.org/A243182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest positive integer representable by 2x^2 + 2xy +
 *            5y^2, considered over all integers x, y (including negative
 *            values). a(1) = 0
 * Range:     u8 n <= 3, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* The build supplies the types seq_t and idx_t. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* Rough upper bound for the n-th value.  Ten times n plus a constant
       is far larger than required for the first 10 000 terms and keeps
       the allocation modest. */
    idx_t bound = 10 * n + 100;
    seq_t bound_seq = (seq_t)bound;

    /* Allocate a byte array to flag which values up to 'bound' appear. */
    uint8_t *seen = (uint8_t *)calloc((size_t)(bound + 1), sizeof *seen);
    if (!seen) return (seq_t)0;  /* allocation failure – return 0 */

    /* Determine maximal |y| such that 5y^2 ≤ bound, and maximal |x| such
       that 2x^2 ≤ bound.  These bounds are small enough that the loop
       counts only a few hundred thousand iterations. */
    idx_t ymax = 0;
    while ((5 * (ymax + 1) * (ymax + 1)) <= bound) ++ymax;
    idx_t xmax = 0;
    while ((2 * (xmax + 1) * (xmax + 1)) <= bound) ++xmax;

    /* Enumerate all integer pairs (x, y) within the calculated bounds
       and mark the representable values.  Negative indices are handled
       by iterating over signed ranges. */
    for (idx_t y = 0; y <= ymax; ++y) {
        /* positive y value */
        seq_t yy = (seq_t)y * (seq_t)y;
        for (idx_t x = 0; x <= xmax; ++x) {
            seq_t xx = (seq_t)x * (seq_t)x;
            seq_t val = 2 * xx + 2 * (seq_t)x * (seq_t)y + 5 * yy;
            if (val > bound_seq) break;
            seen[(size_t)val] = 1;
        }
        /* negative y value (if y != 0) */
        if (y != 0) {
            seq_t yyneg = (seq_t)y * (seq_t)y;  /* y^2 is same for ±y */
            for (idx_t x = 0; x <= xmax; ++x) {
                seq_t xx = (seq_t)x * (seq_t)x;
                seq_t val = 2 * xx - 2 * (seq_t)x * (seq_t)y + 5 * yyneg;
                if (val > bound_seq) break;
                seen[(size_t)val] = 1;
            }
        }
    }

    /* Negative x values: since the form is quadratic in x with an
       odd cross term, we must also evaluate negative x for each y. */
    for (idx_t y = 0; y <= ymax; ++y) {
        seq_t yy = (idx_t)y * (idx_t)y;
        for (idx_t x = 1; x <= xmax; ++x) {
            seq_t xx = (seq_t)x * (seq_t)x;
            /* case x negative, y positive */
            seq_t val = 2 * xx - 2 * (seq_t)x * (seq_t)y + 5 * yy;
            if (val > bound_seq) break;
            seen[(size_t)val] = 1;
            /* case x negative, y negative */
            seq_t val2 = 2 * xx + 2 * (seq_t)x * (seq_t)y + 5 * yy;
            if (val2 > bound_seq) continue;
            seen[(size_t)val2] = 1;
        }
    }

    /* Find the n-th smallest value. */
    idx_t count = 0;
    for (size_t i = 0; i <= (size_t)bound; ++i) {
        if (seen[i]) {
            ++count;
            if (count == n) {
                free(seen);
                return (seq_t)i;
            }
        }
    }

    /* Should not reach here – fall back to maximum found. */
    free(seen);
    return bound_seq;
}
