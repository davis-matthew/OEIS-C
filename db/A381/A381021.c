/*
 * A381021  Expansion of e.g.f. log(1-x)^2 * exp(x) / 2.
 * https://oeis.org/A381021
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * |Stirling1(k,2)|
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The code computes the unsigned Stirling numbers |S(k,2)| on the fly
   using the recurrence         |S(k,2)| = (k-1)*|S(k-1,2)| + (k-2)! ,
   and evaluates the binomial convolution in a single O(n) loop. */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;

    seq_t factorial = (seq_t)1;       /* (k-2)! when k = 2 is 0! = 1    */
    seq_t s_prev1  = (seq_t)0;        /* |S(1,2)| = 0                   */
    seq_t sum      = (seq_t)0;
    seq_t binom    = (seq_t)1;        /* binomial(n,0)                   */

    for (idx_t k = 0; k <= n; ++k) {
        if (k >= 2) {
            /* compute |S(k,2)| = (k-1)*|S(k-1,2)| + (k-2)! */
            seq_t s_k = ((seq_t)(k - 1)) * s_prev1 + factorial;
            sum += binom * s_k;
            s_prev1 = s_k;              /* shift for next iteration     */
            /* factorial for next k becomes (k-1)! */
            factorial *= (seq_t)(k - 1);
        }
        if (k < n) {
            binom = binom * (seq_t)(n - k) / (seq_t)(k + 1); /* next binomial */
        }
    }
    return sum;
}
