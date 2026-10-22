/*
 * A349325  Number of times the Collatz plot started at n crosses the y = n
 *          line, or -1 if the number of crossings is infinite.
 * https://oeis.org/A349325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of times the Collatz plot starting at n crosses
   the y = n line (including the starting point).  The value is
   found by iterating the Collatz trajectory until the value 1 is
   reached. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          // not defined for offset 1, but safe guard
    seq_t count = 1;               // starting point counts as a crossing
    seq_t start = (seq_t)n;        // value of n in seq_t type
    seq_t cur = start;

    while (cur != 1) {
        seq_t nxt;
        if ((cur & 1) != 0) {             // cur is odd
            nxt = (cur * 3 + 1) / 2;
        } else {                          // cur is even
            nxt = cur / 2;
        }

        if ((cur > start && nxt < start) ||
            (cur < start && nxt > start))
            count = count + 1;

        cur = nxt;
    }

    return count;
}
