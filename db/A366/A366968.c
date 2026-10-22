/*
 * A366968  a(n) = Sum_{k=3..n} floor(n/k).
 * https://oeis.org/A366968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=3..n} floor(n/k)
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
/* The types seq_t and idx_t are supplied by the build environment */

seq_t a(idx_t n)
{
    /* For n < 3 the sum is empty */
    if (n < 3) return (seq_t)0;

    seq_t sum = (seq_t)0;
    idx_t k;
    for (k = 3; k <= n; ++k)
    {
        sum += (seq_t)n / (seq_t)k;
    }
    return sum;
}
