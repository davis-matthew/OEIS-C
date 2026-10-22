/*
 * A396724  a(n) = (-4^n/2) p(n, -3/2), where p(n,x) is the Lagrange
 *          interpolating polynomial through (1,0) and (k,1) for k=2..n+1.
 * https://oeis.org/A396724
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: n*a(n)+2*(-4*n-3)*a(n-1)+8*(2*n+3)*a(n-2)=0,
   with initial values a(0)=0, a(1)=5 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)5;

    seq_t prev2 = (seq_t)0;  /* a(0) */
    seq_t prev1 = (seq_t)5;  /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        /* term1 = (8*i + 6)*prev1 */
        seq_t term1 = ((seq_t)(8 * i + 6)) * prev1;
        /* term2 = (16*i + 24)*prev2  == 8*(2*i+3)*prev2 */
        seq_t term2 = ((seq_t)(16 * i + 24)) * prev2;

        /* numerator = term1 - term2 (guaranteed non‑negative) */
        seq_t num = term1 - term2;

        /* cur = num / i   (exact division) */
        cur = num / (seq_t)i;

        /* shift for next iteration */
        prev2 = prev1;
        prev1 = cur;
    }

    return cur;
}
