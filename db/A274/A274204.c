/*
 * A274204  Numbers n such that there is no cubefree number that is the sum of
 *          two nonzero squares in exactly n ways.
 * https://oeis.org/A274204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   is_in_set
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A274204:
 * a(n) is the n-th positive integer that is NOT of the form 2^c * 3^b
 * (c,b ≥ 0) nor of the form (3^b ± 1)/2 (b ≥ 0).
 * These forms are exactly the possible counts of unordered representations
 * of cubefree numbers as the sum of two nonzero squares.
 */

#include <stdbool.h>
#include "../utils/is_in_set.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* out of range, but harmless */

    idx_t missing = 0;
    idx_t val = 1;

    while (true) {
        if (!is_in_set((seq_t)val)) {
            ++missing;
            if (missing == n)
                return (seq_t)val;
        }
        ++val;
    }
}
