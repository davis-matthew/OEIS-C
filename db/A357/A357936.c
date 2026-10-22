/*
 * A357936  a(n) is the least multiple of n that is a Niven (or Harshad)
 *          number.
 * https://oeis.org/A357936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the least multiple of n that is a Niven (Harshad) number
 * Range:     u8 n <= 28, u16 n <= 1498, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: brute‑force search for the smallest k such that n*k is divisible by the digit sum of n*k. */

#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety: undefined for n = 0 */
    seq_t N = (seq_t)n;              /* convert index to seq_t */

    for (idx_t k = 1; ; ++k) {
        seq_t product = N * (seq_t)k;   /* n * k */
        seq_t s = digit_sum(product);   /* digit sum of n*k */
        if (product % s == 0)
            return product;           /* found least multiple */
    }
}
