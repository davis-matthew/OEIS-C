/*
 * A357621  Half-alternating sum of the n-th composition in standard order.
 * https://oeis.org/A357621
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   computes the half-alternating sum of the k‑th composition * in
 *            standard order (graded reverse‑lexicographic, A066099). * The
 *            composition is obtained from the positions of 1's in the *
 *            reversed binary expansion of k, as described in the OEIS entry
 * Range:     i8 n <= 84, i16 n <= 84, i32 n <= 84, i64 n <= 84, i128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Gather the parts of the composition as the first differences
     * of the positions of 1 bits (indices start at 1).  The array
     * parts holds them in the natural order; we later process
     * them in reverse order to apply the half‑alternating sign rule. */
    seq_t parts[65];          /* enough for any 64‑bit integer */
    idx_t count = 0;
    idx_t bit_pos = 1;        /* current bit position (1‑based) */
    idx_t last_pos = 0;       /* position of previous 1 */

    while (n) {
        if (n & 1U) {
            seq_t part = (seq_t)(bit_pos - last_pos);
            parts[count++] = part;
            last_pos = bit_pos;
        }
        n >>= 1;
        ++bit_pos;
    }

    /* Apply the half‑alternating sum (two positives, two negatives, …)
     * in reverse order of the parts array. */
    seq_t sum = 0;
    for (idx_t i = 0; i < count; ++i) {
        seq_t part = parts[count - 1 - i];
        int sgn = ((i / 2) % 2 == 0) ? 1 : -1;
        sum += (seq_t)sgn * part;
    }
    return sum;
}
