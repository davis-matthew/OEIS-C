/*
 * A242043  Pentagonal numbers that are also Niven numbers.
 * https://oeis.org/A242043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 49, u32 n <= 4419, u64 n <= 9730,
 *            u128 n <= 9730, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: iterate pentagonal numbers k*(3k-1)/2 and test the Harshad
   (Niven) property using digit_sum.  The first term is a(1)=1. */
#include <stdbool.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    idx_t k = 1;
    idx_t found = 0;
    seq_t pent;

    while (1) {
        /* Compute k*(3k-1)/2 using seq_t arithmetic to avoid overflow. */
        seq_t kk = (seq_t)k;
        pent = kk * ((seq_t)3 * kk - 1) / 2;

        seq_t s = digit_sum(pent);
        if (s != 0 && pent % s == 0) {
            found++;
            if (found == n)
                return pent;
        }
        k++;
    }
}
