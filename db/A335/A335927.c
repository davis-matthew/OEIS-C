/*
 * A335927  a(n+1) = Sum_{k=1..n} (a(k) + k*(n-k)), with a(1)=1.
 * https://oeis.org/A335927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (9*2^n - 2*n^2 - 6*n - 8)/4 (n > 1), a(1) = 1 The
 *            implementation avoids overflow on 64‑bit builds by dividing the
 *            9*2^n term by 4 before multiplication, and by computing the
 *            subtraction term with a final division by 4. All arithmetic is
 *            performed with the target type seq_t, using the provi...
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    /* Compute (9*2^n)/4  as 9* (2^(n-2))  */
    seq_t term1 = pow2(n);
    term1 >>= 2;          /* divide by 4 */
    term1 *= 9;           /* mult by 9 */

    /* Compute (2*n^2 + 6*n + 8)/4  */
    seq_t n_sq = (seq_t)n * (seq_t)n;
    seq_t part = (seq_t)2 * n_sq;          /* 2*n^2 */
    part += (seq_t)6 * (seq_t)n;           /* +6*n   */
    part += (seq_t)8;                      /* +8     */
    part /= 4;                             

    return term1 - part;
}
