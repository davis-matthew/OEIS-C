/*
 * A318161  Number of compositions of 2n into exactly 2n nonnegative parts
 *          with largest part n.
 * https://oeis.org/A318161
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - 2*n^2 + 2*n*binomial(3*n - 2, n) (for n>0), a(0)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
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

    seq_t nn    = (seq_t)n;
    seq_t bin   = binomial(3 * n - 2, n);

    /* Compute 2*n*bin + n - 2*n^2  using unsigned arithmetic */
    seq_t res   = (seq_t)2 * nn * bin;
    res += nn;
    seq_t sub   = (seq_t)2 * nn * nn;
    res -= sub;

    return res;
}
