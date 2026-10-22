/*
 * A382620  a(n) = n^(2*n-4) * (n!)^2 * (n^2)! * Pochhammer(1+1/n, n-1) /
 *          ((n^2-n+1) * (n^2-n)!).
 * https://oeis.org/A382620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^(2*n-4) * (n!)^2 * (n^2)! * Pochhammer(1+1/n, n-1) /
 *            ((n^2-n+1) * (n^2-n)!) Simplified: a(n) = n^(n-3) * (n!)^2 *
 *            ∏_{t=n^2-n+1}^{n^2} t * ∏_{j=1}^{n-2} (n*j+1) for n ≥ 3
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   factorial, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* small n handled manually */
    if (n == 1) return 1;
    if (n == 2) return 24;

    /* n^(n-3) */
    seq_t pow_term = seq_pow((seq_t)n, n - 3);

    /* (n!)^2 */
    seq_t fact_n = factorial(n);
    seq_t fact_n_sq = seq_mul(fact_n, fact_n);

    /* product ∏_{t=n^2-n+1}^{n^2} t  (n terms) */
    idx_t start_t = n * n - n + 1;
    idx_t end_t   = n * n;
    seq_t prod_t = 1;
    for (idx_t t = start_t; t <= end_t; ++t) {
        prod_t = seq_mul(prod_t, (seq_t)t);
    }

    /* product ∏_{j=1}^{n-2} (n*j+1)  (n-2 terms) */
    seq_t prod_j = 1;
    for (idx_t j = 1; j <= n - 2; ++j) {
        prod_j = seq_mul(prod_j, (seq_t)(n * j + 1));
    }

    /* combine all factors */
    seq_t result = pow_term;
    result = seq_mul(result, fact_n_sq);
    result = seq_mul(result, prod_t);
    result = seq_mul(result, prod_j);

    return result;
}
