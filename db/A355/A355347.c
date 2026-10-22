/*
 * A355347  a(n) = binomial(3*n+3,n) + binomial(3*n+2,n-1) for n >= 0.
 * https://oeis.org/A355347
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(3*n+3, n) + binomial(3*n+2, n-1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binomial_ui
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial_ui.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    idx_t three_n  = 3 * n;
    seq_t t1 = binomial_ui(three_n + 3, n);          /* binomial(3n+3, n) */
    seq_t t2 = binomial_ui(three_n + 2, n - 1);      /* binomial(3n+2, n-1) */

    return t1 + t2;
}
