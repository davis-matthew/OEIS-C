/*
 * A292732  Numbers in which 2 outnumbers all other digits together.
 * https://oeis.org/A292732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   majority_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for numbers whose digit 2 occurs more often than all other digits combined. */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/majority_two.h"

/* returns a(n) for n >= 1 */
seq_t a(idx_t n) {
    if (n == (idx_t)0)
        return (seq_t)0;   /* undefined for n=0, return 0 as a placeholder */

    seq_t current = (seq_t)2;
    size_t found = 0;
    size_t target = (size_t)n;   /* n is assumed small enough for this cast */

    while (found < target) {
        if (majority_two(current)) {
            ++found;
            if (found == target)
                return current;
        }
        ++current;
    }
    return (seq_t)0; /* unreachable */
}
