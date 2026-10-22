/*
 * A302116  a(n) = 16*(n-1)*((2*n-3)*a(n-1) + (((-1)^n)/9)*Product_{k=0..n-1}
 *          (2*k-3)^2) with a(0) = 0.
 * https://oeis.org/A302116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A302116
 * Recurrence:  a(0)=0, a(1)=0, a(2)=16
 * For n>=3:
 * (n-2)*a(n) = (n-1)*(28*n^2-92*n+71)*a(n-1) + 16*(n-2)*(n-1)*(2*n-5)^3*a(n-2)
 * This function returns a(n) using the above recurrence. */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0u || n == 1u) return 0;
    if (n == 2u) return (seq_t)16;

    seq_t a_prev2 = 0;   /* a(1) */
    seq_t a_prev1 = (seq_t)16;  /* a(2) */

    for (idx_t i = 3u; i <= n; ++i) {
        /* term1 = (i-1)*(28*i^2 - 92*i + 71)*a_prev1 */
        seq_t term1_factor = (seq_t)(i - 1u) *
                             (seq_t)((28ULL * (i * i)) - (92ULL * i) + 71ULL);
        seq_t term1 = term1_factor * a_prev1;

        /* term2 = 16*(i-2)*(i-1)*(2*i-5)^3*a_prev2 */
        seq_t t = (seq_t)(2ULL * i - 5ULL);
        seq_t t3 = t * t * t;                 /* (2*i-5)^3 */
        seq_t term2_factor = (seq_t)16ULL *
                             (seq_t)(i - 2u) *
                             (seq_t)(i - 1u);
        seq_t term2 = term2_factor * t3 * a_prev2;

        seq_t numerator = term1 + term2;
        seq_t denom = (seq_t)(i - 2u);
        seq_t a_i = numerator / denom;

        a_prev2 = a_prev1;
        a_prev1 = a_i;
    }

    return a_prev1;
}
