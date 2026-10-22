/*
 * A360797  Expansion of Sum_{k>0} x^k / (1 - 2 * x^k)^(k+1).
 * https://oeis.org/A360797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{d|n} 2^(n/d-1) * binomial(d + n/d - 1, d)
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1, result for 0 is unused */
    seq_t sum = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            /* divisor i */
            idx_t d = i;
            idx_t k = n / d;
            seq_t term = binomial(d + k - 1, d);
            term <<= (k - 1);      /* multiply by 2^(k-1) */
            sum += term;

            /* paired divisor n/i, avoid double‑counting square root */
            if (i != n / i) {
                d = n / i;        /* the larger divisor */
                k = n / d;        /* which equals i */
                term = binomial(d + k - 1, d);
                term <<= (k - 1);
                sum += term;
            }
        }
    }
    return sum;
}
