/*
 * A372435  Length of the shortest binary string that contains all blocks of
 *          the form xx, where x is a length-n string.
 * https://oeis.org/A372435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = (n == 1 ? 4 : 3 * 2^n)
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)4;

    // Calculate 3 * 2^n using the provided pow2 utility
    return 3 * pow2(n);
}
