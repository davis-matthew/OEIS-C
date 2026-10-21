/*
 * A255589  Convert n to base 4, move the least significant digit to the most
 *          significant one and convert back to base 10.
 * https://oeis.org/A255589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = (n/4) + (n%4) * 4^(m-1), where m is the number of base‑4
 *            digits of n. This implements the “move the least significant
 *            base‑4 digit to the most significant position and convert back
 *            to base‑10” transformation
 * Range:     u8 n <= 256, u16 n <= 7824, u32 n <= 7824, u64 n <= 7824,
 *            u128 n <= 7824, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Handle n == 0 explicitly – the loop logic works for 0 as well,
       but this makes the special case visible. */
    if (n == 0) {
        return 0;
    }

    /* Least significant base‑4 digit and n divided by 4. */
    idx_t d0 = n % 4;
    idx_t n_div = n / 4;

    /* Count the number of base‑4 digits of n. */
    idx_t tmp = n;
    idx_t m = 1;
    while (tmp >= 4) {
        tmp /= 4;
        ++m;
    }

    /* Compute 4^(m-1) safely within seq_t. */
    seq_t pow4 = 1;
    for (idx_t i = 0; i < m - 1; ++i) {
        pow4 *= 4;
    }

    /* Final result: rotate the least significant digit to the front. */
    return (seq_t)n_div + (seq_t)d0 * pow4;
}
