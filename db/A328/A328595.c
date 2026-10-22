/*
 * A328595  Numbers whose reversed binary expansion is a necklace.
 * https://oeis.org/A328595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   iterative search for numbers whose reversed binary expansion is
 *            a necklace. The function a(n) returns the nth term in ascending
 *            order
 * Range:     u8 n <= 85, u16 n <= 8907, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_necklace_rev_bin
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_necklace_rev_bin.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* not used, but guard against underflow */
    idx_t count = 0;
    idx_t candidate = 1;
    while (1) {
        if (is_necklace_rev_bin(candidate)) {
            ++count;
            if (count == n) return (seq_t)candidate;
        }
        ++candidate;
    }
}
