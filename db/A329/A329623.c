/*
 * A329623  The absolute value of the difference between n and A053392(n), the
 *          concatenation of the sums of every pair of consecutive digits of
 *          n.
 * https://oeis.org/A329623
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = | n - A053392(n) | where A053392(n) is the concatenation
 *            of sums of every pair of consecutive digits of n
 * Range:     u8 n <= 136, u16 n <= 1645, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _A053392
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/_A053392.h"

seq_t a(idx_t n)
{
    seq_t val = (seq_t)n;
    seq_t con = _A053392(n);
    return (val >= con) ? (val - con) : (con - val);
}
