/*
 * A294389  a(n) = 2^(n-3) mod n, for n >= 3.
 * https://oeis.org/A294389
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n-3) mod n
 * Range:     u8 n <= 296, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;
    return mod_pow((seq_t)2, n - 3, (seq_t)n);
}
