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
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A354227: odd numbers whose Collatz trajectory contains exactly 12 odd numbers. */

#include <stdbool.h>  /* for bool */

/* Count how many odd numbers appear in the Collatz trajectory of ‘x’.
   The starting number itself is counted.  Stops early if the count
   exceeds 12 to keep the search fast. */
static seq_t octrajectory_count(seq_t x)
{
    seq_t odd_cnt = 1;          /* the starting number is odd */
    while (x != 1) {
        if (x & 1) {            /* odd step: 3x + 1 */
            x = 3 * x + 1;
        } else {                /* even step: divide by 2 until odd or 1 */
            while ((x & 1) == 0 && x != 1) {
                x >>= 1;
            }
        }
        if (x & 1) {            /* just landed on an odd number */
            ++odd_cnt;
            if (odd_cnt > 12) return odd_cnt;   /* early exit */
        }
    }
    return odd_cnt;
}

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
