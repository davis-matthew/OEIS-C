/*
 * A272344  Positive integers n where the number of parts function on the set
 *          of 3-ary partitions of n is equidistributed mod 3.
 * https://oeis.org/A272344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A272344: positive integers n whose base‑3 expansion contains a digit
   2 at some position other than the units place. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* offset starts at 1, but guard against 0 */

    idx_t count = 0;
    idx_t candidate = 0;

    while (count < n) {
        candidate++;                       /* test next integer */

        /* skip the least significant digit and look for a 2 in a higher place */
        idx_t t = candidate / 3;
        bool has_two = false;
        while (t > 0) {
            if ((t % 3) == 2) { has_two = true; break; }
            t /= 3;
        }
        if (has_two) ++count;              /* found a term of the sequence */
    }

    return (seq_t)candidate;
}
