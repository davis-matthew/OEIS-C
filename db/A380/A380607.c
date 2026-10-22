/*
 * A380607  a(0) = 1, a(n) = 5*binomial(6*(n-1),n-1), for n > 0.
 * https://oeis.org/A380607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*binomial(6*(n-1), n-1) for n>0, a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;
    idx_t k = n - 1;
    idx_t N = 6 * k;
    seq_t val = binomial(N, k);
    val *= 5;
    return val;
}
