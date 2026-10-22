/*
 * A386938  a(n) = Sum_{k=0..n} binomial(4*n+1,k) * binomial(2*n-k-1,n-k).
 * https://oeis.org/A386938
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   binomial_integer
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial_integer.h"
/* OEIS A386938 */
/* a(n) = Sum_{k=0..n} binomial(4*n+1,k) * binomial(2*n-k-1,n-k)   */

/* The helper binomial calculation is local, so no external symbol is
   required.  It uses the multiplicative formula and works for any
   seq_t-sized result. */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t sum = (seq_t)0;
    seq_t a_k = (seq_t)1;                                   /* C(4*n+1,0) */
    seq_t b_k = binomial_integer(2*n - 1, n);               /* C(2*n-1,n) */

    for (idx_t k = 0; k <= n; ++k) {
        sum += a_k * b_k;

        if (k == n) break;                                 /* last term done */

        /* update a_k to C(4*n+1,k+1) */
        seq_t num_a = (seq_t)(4*n + 1 - k);
        seq_t den_a = (seq_t)(k + 1);
        a_k = a_k * num_a / den_a;

        /* update b_k to C(2*n-k-2, n-k-1) */
        idx_t r_k = n - k;                                 /* n-k >= 1 */
        seq_t m_k = (seq_t)(2*n - k - 1);
        b_k = b_k * (seq_t)r_k / m_k;
    }
    return sum;
}
