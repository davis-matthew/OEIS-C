/*
 * A292736  Numbers in which 6 outnumbers all other digits together.
 * https://oeis.org/A292736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 395, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_majority_six
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for OEIS A292736: Numbers in which the digit 6 outnumbers
 * all other digits together.  The value is found by brute‑force scanning of
 * integers, checking for the digit–count condition.  The function works
 * for any build where `idx_t` is an unsigned integer type (uint64_t).
 * It returns the result as a `seq_t`. */
#include <stdbool.h>
#include "../utils/has_majority_six.h"

seq_t a(idx_t n)
{
    if (n == 0)   /* offset starts at 1; 0 is undefined, return 0 */
        return (seq_t)0;

    idx_t found = 0;
    for (idx_t x = 6; ; ++x) {
        if (has_majority_six(x)) {
            ++found;
            if (found == n)
                return (seq_t)x;
        }
    }
}
