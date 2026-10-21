/*
 * A250768  Number of (n+1) X (7+1) 0..1 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing absolute
 *          value of x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 81*2^(n-1) + 198*n + 240
 * Range:     u8 none, u16 n <= 10, u32 n <= 26, u64 n <= 58, u128 n <= 122,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* compute 81*2^(n-1) + 198*n + 240 */
    seq_t pow2   = seq_pow((seq_t)2, n - 1);
    seq_t result = seq_mul((seq_t)81, pow2);
    result = result + seq_mul((seq_t)198, (seq_t)n);
    result = result + (seq_t)240;
    return result;
}
