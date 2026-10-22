/*
 * A383678  a(n) = [x^n] Product_{k=0..n} (1 + (2*n+k)*x).
 * https://oeis.org/A383678
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^n] ∏_{k=0..n} (1 + (2*n + k) * x)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the offset case */
    if (n == 0)
        return (seq_t)1;

    /* Compute product ∏_{i=2n}^{3n} i */
    idx_t start = 2 * n;          /* 2*n  */
    idx_t end   = 3 * n;          /* 3*n  */
    seq_t prod = (seq_t)1;
    for (idx_t i = start; i <= end; ++i)
        prod *= (seq_t)i;

    /* Sum over the missing‑factor terms */
    seq_t sum   = (seq_t)0;
    seq_t denom = (seq_t)start;  /* 2*n  */
    for (idx_t j = 0; j <= n; ++j)
        sum += prod / (denom + (seq_t)j);

    return sum;
}
