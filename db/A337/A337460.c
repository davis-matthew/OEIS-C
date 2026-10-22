/*
 * A337460  Numbers k such that the k-th composition in standard order is a
 *          non-unimodal triple.
 * https://oeis.org/A337460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A337460 –   numbers k such that the k‑th composition (graded
   reverse‑lexicographic) is a valley triple (a>b and c>b).
   The function computes this by iterating over k, converting each k to
   its composition via the binary bit‑position algorithm described in
   the OEIS entry, and testing whether the composition has length 3
   and satisfies the valley condition.  */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* sequence is defined for n ≥ 1 */

    idx_t k = 1;                         /* start with the first non‑zero index */
    idx_t found = 0;

    for (;;) {
        /* Convert k to its composition (array of positive parts). */
        int part_count = 0;
        int prev_pos   = 0;              /* previous position of a 1‑bit */
        int diffs[3];                    /* store diffs of the first three 1‑bits */
        int bit_pos   = 1;               /* 1‑based bit position (LSB position 1) */
        idx_t t = k;
        int over = 0;                    /* flag when more than three parts */

        while (t) {
            if (t & 1ULL) {
                if (part_count < 3) {
                    diffs[part_count] = bit_pos - prev_pos;
                }
                part_count++;
                prev_pos = bit_pos;

                if (part_count > 3) {
                    over = 1;            /* more than three parts – cannot be a triple */
                    break;
                }
            }
            t >>= 1;
            bit_pos++;
        }

        if (!over && part_count == 3) {
            /* The composition is the reverse of `diffs`.  Therefore:
               a = diffs[2], b = diffs[1], c = diffs[0]             */
            int a_part = diffs[2];
            int b_part = diffs[1];
            int c_part = diffs[0];
            if (a_part > b_part && c_part > b_part) {
                found++;
                if (found == n) return (seq_t)k;
            }
        }

        k++;                             /* next integer to test */
    }
}
