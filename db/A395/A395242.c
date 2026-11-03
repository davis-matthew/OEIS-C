/*
 * A395242  Numbers whose binary representation contains isolated digits "11"
 *          at least once.
 * https://oeis.org/A395242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth positive integer whose binary representation
 *            contains at least one isolated run of exactly two consecutive 1
 *            bits
 * Range:     u8 n <= 120, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 8033 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Return true if x has an isolated run of exactly two 1's in its binary representation.
   An isolated run is a pattern 11 that is surrounded on both sides by 0 or by a
   bit boundary. */
static bool has_isolated_11(seq_t x)
{
    unsigned run = 0;
    while (x) {
        if (x & (seq_t)1) {
            run++;
        } else {
            if (run == 2)
                return true;          /* isolated 11 found */
            run = 0;                  /* reset after a 0 */
        }
        x >>= 1;
    }
    return run == 2;                    /* isolated 11 at the most‑significant side? */
}

seq_t a(idx_t n)
{
    /* Scan forward until we have found the n‑th term. */
    seq_t cand = 1;
    idx_t count = 0;

    while (1) {
        if (has_isolated_11(cand)) {
            ++count;
            if (count == n)
                return cand;
        }
        ++cand;
    }
}
