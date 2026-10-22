/*
 * A354227  Odd numbers whose Collatz trajectory contains exactly 12 odd
 *          numbers.
 * https://oeis.org/A354227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   octrajectory_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A354227: odd numbers whose Collatz trajectory contains exactly 12 odd numbers. */

#include <stdbool.h>  /* for bool */
#include "../utils/octrajectory_count.h"

/* Return the n‑th term of the sequence (n ≥ 1).  The function simply
   scans successive odd integers and counts those whose Collatz trajectory
   has exactly 12 odd numbers.  No caching is used because the search
   is fast enough for all n that appear in the OEIS b‑file. */
seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t cand = 1;                 /* first odd integer */

    while (found < n) {
        if (octrajectory_count(cand) == 12) {
            ++found;
            if (found == n) return cand;
        }
        cand += 2;                  /* next odd integer */
    }
    /* Unreachable, but keep the compiler happy */
    return 0;
}
