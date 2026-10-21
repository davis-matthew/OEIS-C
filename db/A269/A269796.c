/*
 * A269796  a(n) = 4^n * A000108(n+1).
 * https://oeis.org/A269796
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n * Catalan(n+1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   catalan, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow4.h"
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    return pow4(n) * catalan(n + 1);
}
