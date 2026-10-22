/*
 * A366288  Numbers k such that 3*k is not squarefree.
 * https://oeis.org/A366288
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th positive integer k such that 3*k is not
 *            squarefree. Computed by scanning integers and testing for a
 *            prime square factor or divisibility by 3
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   has_square_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/has_square_factor.h"

seq_t a(idx_t n)
{
    uint64_t count = 0;
    uint64_t k = 1;
    while (1) {
        if ((k % 3 == 0) || has_square_factor(k)) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
        ++k;
    }
}
