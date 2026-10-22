/*
 * A323217  a(n) = hypergeometric([-n, n + 1], [-n - 1], n + 1).
 * https://oeis.org/A323217
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..n} (binomial(2*n-j, n) - binomial(2*n-j,
 *            n+1))*(n+1)^(n-j)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   binom, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t base = (seq_t)n + 1;
    /* power holds (n+1)^(n-j); start with j=0 */
    seq_t power = pow_uint((uint64_t)base, n);

    for (idx_t j = 0; j <= n; ++j) {
        seq_t bin1 = binom((idx_t)(2 * n - j), (idx_t)n);
        seq_t bin2 = binom((idx_t)(2 * n - j), (idx_t)(n + 1));
        seq_t coeff = bin1 - bin2;          /* non‑negative */
        sum += coeff * power;               /* exact integer multiplication */
        /* Prepare power for next j+1: divide by base (n+1) */
        power /= base;                      /* exact division */
    }
    return sum;
}
