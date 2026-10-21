/*
 * A281907  a(n) = 66483034025018711639862527490*n +
 *          47867742232066880047611079.
 * https://oeis.org/A281907
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 66483034025018711639862527490*n +
 *            47867742232066880047611079
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 n <= 7, big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    static seq_t coeff;
    static seq_t constant;
    static bool initialized = false;
    if (!initialized) {
        coeff = seq_from_str("66483034025018711639862527490");
        constant = seq_from_str("47867742232066880047611079");
        initialized = true;
    }
    seq_t result = coeff * (seq_t)n;
    result += constant;
    return result;
}
