/*
 * A249367  Numbers of the form 2n^2 or 3n^2.
 * https://oeis.org/A249367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A249367: sorted union of 2*n^2 and 3*n^2, starting with 0. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Return the nth element (1‑indexed) of the sequence.*/
    if (n < 1) return 0;                 /* undefined for n<1, safe fallback */

    idx_t i2 = 0, i3 = 0;                 /* indices in 2*k^2 and 3*k^2 sequences */
    idx_t count = 0;                      /* how many distinct values seen */
    seq_t prev = 0, curr = 0;             /* previous and current value */

    while (count < n) {
        seq_t val2 = 2 * ((seq_t)i2 * (seq_t)i2); /* 2*i2^2 */
        seq_t val3 = 3 * ((seq_t)i3 * (seq_t)i3); /* 3*i3^2 */

        if (val2 < val3) {
            curr = val2;
            ++i2;
        } else if (val3 < val2) {
            curr = val3;
            ++i3;
        } else {                          /* val2 == val3 (only occurs at 0) */
            curr = val2;
            ++i2;
            ++i3;
        }

        /* Skip duplicate 0 only once; all other values are unique. */
        if (count == 0 || curr != prev) {
            prev = curr;
            ++count;
        }
    }
    return curr;
}
