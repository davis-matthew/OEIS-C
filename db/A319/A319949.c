/*
 * A319949  a(n) = Product_{i=1..n} floor(4*i/3).
 * https://oeis.org/A319949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{i=1..n} ⌊4*i/3⌋ (offset 1)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* a(0) is out of bounds for the OEIS offset; return 0 for safety */
    if (n == 0)
        return (seq_t)0;

    seq_t prod = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t mult = ((seq_t)4 * (seq_t)i) / (seq_t)3;
        prod *= mult;
    }
    return prod;
}
