/*
 * A359952  Wieferich sequence where a(1) = 2.
 * https://oeis.org/A359952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition:
   a(1) = 2,
   a(2) = 1093,
   for n >= 3 the sequence repeats the cycle
   5, 20771, 18043. */
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)1093;
    static const seq_t cycle[3] = {(seq_t)5, (seq_t)20771, (seq_t)18043};
    return cycle[(size_t)((n - 3) % 3)];
}
