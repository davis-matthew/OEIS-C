/*
 * A387755  a(n) = (1/4) * Sum_{k=0..n-1} binomial(10*n,10*k+5).
 * https://oeis.org/A387755
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/4) * Sum_{k=0..n-1} binomial(10*n,10*k+5)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t sum = 0;
    idx_t N = 10 * n;                 /* N = 10*n */
    for (idx_t k = 0; k < n; ++k) {
        idx_t r = 10 * k + 5;          /* r = 10*k + 5 */
        seq_t term = binomial(N, r);   /* binomial(10*n, 10*k+5) */
        sum += term;
    }
    return sum / 4;                    /* divide by 4 to finish */
}
