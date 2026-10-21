/*
 * A245806  a(n) = 3^n + 10^n.
 * https://oeis.org/A245806
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n + 10^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   pow10, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Computes 3^n + 10^n exactly. */
    return pow3(n) + pow10(n);
}
