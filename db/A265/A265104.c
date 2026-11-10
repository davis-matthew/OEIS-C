/*
 * A265104  a(n) = A265100(n+1) - 6, n >= 1.
 * https://oeis.org/A265104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A265100(n+1) - 6 = 9*A005836(n+1) - 1, where A005836(n+1)
 *            is the binary expansion of n read in base 3
 * Range:     u8 n <= 9, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t x = n;
    seq_t res = 0;
    seq_t p3 = 1;
    while (x) {
        if (x & 1U)
            res += p3;
        p3 *= 3;
        x >>= 1;
    }
    return res * (seq_t)9 - (seq_t)1;
}
