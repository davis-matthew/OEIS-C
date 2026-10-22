/*
 * A347269  a(n) is the first term of the n-th 3x+1 sequence that shares
 *          infinitely many 1's with the 3x+1 sequence that starts at 4.
 * https://oeis.org/A347269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th positive integer whose first appearance of 1 in
 *            its Collatz (3x+1) trajectory occurs on a step congruent to 2
 *            modulo 3
 * Range:     u8 n <= 11, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t found = 0;
    idx_t candidate = 1;

    while (found < n) {
        seq_t cur = (seq_t)candidate;
        idx_t step_mod = 0;

        while (cur != (seq_t)1) {
            if ((cur % (seq_t)2) == (seq_t)0)
                cur = cur / (seq_t)2;
            else
                cur = cur * (seq_t)3 + (seq_t)1;
            step_mod = (step_mod + 1) % 3;
        }

        if (step_mod == (idx_t)2) {
            ++found;
            if (found == n)
                return (seq_t)candidate;
        }
        ++candidate;
    }

    /* Should never be reached for valid n >= 1. */
    return (seq_t)0;
}
