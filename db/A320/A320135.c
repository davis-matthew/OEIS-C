/*
 * A320135  Number of integer solutions to a^2 + b^2 + 2*c^2 + 13*d^2 = n.
 * https://oeis.org/A320135
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) equals the number of integer solutions to a^2 + b^2 + 2c^2
 *            + 13d^2 = n
 * Range:     u8 n <= 88, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   r2_squares
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/r2_squares.h"

seq_t a(idx_t n)
{
    seq_t total = (seq_t)0;
    for (idx_t d = 0; ; ++d) {
        idx_t d2 = d * d;
        idx_t term13 = 13 * d2;
        if (term13 > n)
            break;
        seq_t dmult = (d == 0 ? (seq_t)1 : (seq_t)2);

        idx_t rem1 = n - term13;
        for (idx_t c = 0; ; ++c) {
            idx_t c2 = c * c;
            idx_t term2 = 2 * c2;
            if (term2 > rem1)
                break;
            seq_t cmult = (c == 0 ? (seq_t)1 : (seq_t)2);

            idx_t rem2 = rem1 - term2;
            seq_t r = r2_squares(rem2);
            total += r * (dmult * cmult);
        }
    }
    return total;
}
