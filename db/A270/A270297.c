/*
 * A270297  Numbers which are representable as a sum of seven but no fewer
 *          consecutive nonnegative integers.
 * https://oeis.org/A270297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 1248, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers representable as a sum of seven consecutive nonnegative integers.
   a(n) = 28*A229829(n), where A229829(n) is the n-th positive integer that is not a
   multiple of 3 or 5. */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* The sequence is 1‑based.  n==0 is unused by the OEIS offset. */
    if (n == 0) return 0;

    idx_t count = 0;
    idx_t i = 1;          /* candidate integer for A229829 */
    seq_t result = 0;

    while (count < n) {
        if ((i % 3) != 0 && (i % 5) != 0) {
            ++count;
            if (count == n) {
                result = (seq_t)i * (seq_t)28;
                break;
            }
        }
        ++i;
    }

    return result;
}
