/*
 * A339890  Number of odd-length factorizations of n into factors > 1.
 * https://oeis.org/A339890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered factorizations of n into factors > 1
 *            with odd length
 * Range:     u8 n <= 8639, u16 n <= 9660, u32 n <= 9660, u64 n <= 9660,
 *            u128 n <= 9660, big any n
 * Helpers:   count_odd_even
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/count_odd_even.h"

seq_t a(idx_t n)
{
    if (n <= 1) return 0;
    seq_t odd = 0;
    seq_t even = 0;
    count_odd_even(n, 2, &odd, &even);
    return odd;
}
