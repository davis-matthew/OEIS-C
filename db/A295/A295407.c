/*
 * A295407  a(n) = n! * Laguerre(n, 3*n, -n).
 * https://oeis.org/A295407
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n!*Sum_{k=0..n} binomial(4*n,n-k)*n^k/k!
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 19,
 *            big any n
 * Helpers:   binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses a recurrence that keeps intermediate values in the natural range. */

#include "../utils/binomial.h"
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    /* n! */
    seq_t fact_n = factorial(n);

    /* Initial term: k = 0  →  binomial(4n,n) * n^0 * n! */
    seq_t term = binomial(4 * n, n) * fact_n;

    /* Sum of all terms */
    seq_t sum = term;

    /* Recurrence: T_{k+1} = T_k * (n-k) / (3n+k+1) * n / (k+1) */
    for (idx_t k = 0; k < n; ++k) {
        term = term * (seq_t)(n - k);
        term = term / (seq_t)(3 * n + k + 1);
        term = term * (seq_t)n;
        term = term / (seq_t)(k + 1);
        sum += term;
    }

    return sum;
}
