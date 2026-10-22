/*
 * A309761  Numbers that are sums of consecutive powers of 10.
 * https://oeis.org/A309761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (10^i - 10^(i-j))/9 where i*(i-1)/2 + j = n (equivalently
 *            i = minimal with i*(i+1)/2 ≥ n, j = n - (i-1)*i/2)
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 55, u64 n <= 210, u128 n <= 780,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

extern seq_t a(idx_t n)
{
    /* Find minimal i such that i*(i+1)/2 >= n */
    idx_t i = 1;
    idx_t sum = 1;          /* sum = i*(i+1)/2 for current i */
    while (sum < n) {
        i++;
        sum += i;
    }

    /* j in 1..i */
    idx_t j = n - ((i - 1) * i) / 2;

    /* Compute repunit = 111...1 (j times) */
    seq_t repunit = 0;
    for (idx_t k = 0; k < j; ++k)
        repunit = repunit * 10 + 1;

    /* Multiply by 10^(i-j) to append zeros */
    seq_t multiplier = 1;
    for (idx_t k = 0; k < i - j; ++k)
        multiplier *= 10;

    return repunit * multiplier;
}
