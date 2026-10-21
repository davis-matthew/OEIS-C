/*
 * A280934  Number of 2 X 2 matrices with all elements in {0,..,n} and (sum of
 *          terms) = permanent.
 * https://oeis.org/A280934
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*n - 20 + 4*∑_{k=1}^{n+1} ⌊(n+1)/k⌋ for n ≥ 3; a(0)=1,
 *            a(1)=1, a(2)=4
 * Range:     u8 n <= 13, u16 n <= 1706, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first three terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;

    /* Compute the sum of floor divisions */
    idx_t m = n + 1;
    seq_t sum = 0;
    idx_t i = 1;
    while (i <= m) {
        idx_t q = m / i;          /* q = floor(m / i) */
        idx_t last = m / q;       /* last such that floor(m / j) = q for i ≤ j ≤ last */
        sum += (seq_t)q * (last - i + 1);
        i = last + 1;
    }

    /* Final formula */
    return (seq_t)8 * n - (seq_t)20 + (seq_t)4 * sum;
}
