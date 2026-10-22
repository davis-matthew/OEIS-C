/*
 * A329113  Row sums of A329058.
 * https://oeis.org/A329113
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(1 + 3*n, n)*2F1([1, -n], [-1 - 3*n], 1 + n)/(1 +
 *            n)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"     /* provides seq_t binom(idx_t a, idx_t b) */

seq_t a(idx_t n)
{
    /*  Use the identity
            a(n) = Σ_{k=0}^n binom(3n+1-k, n-k) * (n+1)^{k-1}
       where the term for k=0 is binom(3n+1, n)/(n+1).              */

    /* Compute the first binomial coefficient */
    seq_t B = binom(3*n + 1, n);        /* B = binom(3n+1, n)          */
    seq_t result = B / (n + 1);         /* term k = 0                   */
    seq_t pow = (seq_t)1;              /* (n+1)^{k-1} for k=1         */

    /* Iterate over k = 1 … n, updating B and accumulating terms */
    for (idx_t k = 1; k <= n; ++k) {
        /* Update B = binom(3n+1-k, n-k) using:  B_{k} = B_{k-1} * (n-k+1)/(3n+2-k)  */
        seq_t num = (n - (k - 1));           /* n - (k-1) = n - k + 1        */
        seq_t den = (3*n + 2 - k);           /* 3n + 2 - k                   */
        B = (B * num) / den;                /* exact division */

        /* Accumulate term for current k */
        result += B * pow;                  /* term_k = B_k * (n+1)^{k-1}   */

        /* Prepare pow for next k (multiply by n+1) */
        pow *= (seq_t)(n + 1);
    }

    return result;
}
