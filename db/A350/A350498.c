/*
 * A350498  Convolution of triangular numbers with every third number of
 *          Narayana's Cows sequence.
 * https://oeis.org/A350498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = N(3n-1) - n*(n+1)/2 where N(k) is Narayana's Cows
 *            sequence: N(0)=0, N(1)=N(2)=1, N(3)=2, and for k≥4,
 *            N(k)=N(k-1)+N(k-3)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   narayana_cow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/narayana_cow.h"

seq_t a(idx_t n)
{
    /* n ≥ 1 as per the OEIS offset */
    idx_t k = 3 * n - 1;
    seq_t N_val = narayana_cow(k);
    seq_t tri   = ((seq_t)n * ((seq_t)n + 1)) / 2;
    return N_val - tri;
}
