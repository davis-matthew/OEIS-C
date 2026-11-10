/*
 * A382836  Antidiagonal sums of A382835.
 * https://oeis.org/A382836
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} A(n-k,k), where A(r,c) =
 *            (6*r+1)*(12*r+1)*Product_{i=1..c} (9*2^i*r+1) (A382835)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 14 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t A382836_cell(idx_t r, idx_t c)
{
    seq_t R = (seq_t)r;
    seq_t v = (6 * R + 1) * (12 * R + 1);
    seq_t p2 = 1;
    for (idx_t i = 1; i <= c; ++i)
    {
        p2 *= 2;
        v *= (seq_t)9 * p2 * R + 1;
    }
    return v;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k)
        sum += A382836_cell(n - k, k);
    return sum;
}
