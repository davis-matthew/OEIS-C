/*
 * A384189  Numbers whose number of zeros in their binary representation is
 *          not equal to 1.
 * https://oeis.org/A384189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth positive integer m such that the number of zeros
 *            in the binary representation of m is NOT equal to 1. The
 *            sequence is thus the complement of numbers with exactly one zero
 *            in binary (e.g., 2, 4, 8, 16, 32, …)
 * Range:     u8 n <= 227, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_zero_count_other_than_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/has_zero_count_other_than_one.h"

seq_t a(idx_t n)
{
    /* Handle the off‑by‑one: a(1) is the first term.   */
    if (n == 0)
        return (seq_t)0;                      /* undefined slot */

    idx_t candidate = 1;       /* start from 1 */
    idx_t count     = 0;       /* how many valid terms seen so far */

    while (count < n) {
        if (has_zero_count_other_than_one(candidate)) {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }
        ++candidate;
    }

    /* Should never reach here, but keep compiler happy. */
    return (seq_t)0;
}
