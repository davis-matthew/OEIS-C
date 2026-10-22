/*
 * A375227  Expansion of e.g.f. exp( (x / (1-x))^3 ) / (1-x)^3.
 * https://oeis.org/A375227
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n!*Sum_{k=0..floor(n/3)} binomial(n+2,n-3k)/k!
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/binomial.h"    /* seq_t binomial(idx_t a, idx_t b) */
#include "../utils/factorial.h"   /* seq_t factorial(idx_t n) */

/* sjseq: sequence for OEIS A375227 */
seq_t a(idx_t n)
{
    /* n!  */
    seq_t fact_n = factorial(n);
    /* current factorial of k (k!); start with 0! = 1 */
    seq_t fact_k = (seq_t)1;
    seq_t result = (seq_t)0;

    idx_t max_k = n / 3;
    for (idx_t k = 0; k <= max_k; ++k) {
        idx_t choose = n - 3 * k;               /* n-3k  */
        seq_t binom = binomial(n + 2, choose);  /* C(n+2, n-3k) */
        seq_t term = fact_n * binom / fact_k;   /* exactly divisible */
        result += term;
        /* update k! for next iteration (k+1)! = k! * (k+1) */
        fact_k *= (k + 1);
    }
    return result;
}
