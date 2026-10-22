/*
 * A382733  Number of hypercubic bipartitions of n.
 * https://oeis.org/A382733
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(2^ceil(log2(n)) - n) + 1, with a(0) = a(1) = 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   smallest_power_of_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for bitwise operations on idx_t */
#include "../utils/smallest_power_of_two.h"

/* OEIS sequence A382733 */
seq_t a(idx_t n)
{
    if (n <= 1)                     /* a(0) = a(1) = 1 */
        return 1;

    idx_t steps = 0;
    while (n > 1) {
        idx_t p = smallest_power_of_two(n);   /* 2^ceil(log2(n)) */
        n   = p - n;
        steps++;
    }
    return (seq_t)(steps + 1);
}
