/*
 * A294390  a(n) = 2^(n-4) mod n, for n >= 4.
 * https://oeis.org/A294390
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(n) = 2^(n-4) mod n
 * Range:     u8 n <= 274, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n < 4) return 0;          /* undefined for n < 4 */
    return mod_pow((seq_t)2, n - 4, (seq_t)n);
}
