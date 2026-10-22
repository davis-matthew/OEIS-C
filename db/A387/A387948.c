/*
 * A387948  E.g.f. A(x) satisfies A(x) = exp( x^2*A(x)^2 / (1 - x*A(x))^2 ).
 * https://oeis.org/A387948
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=1}^{⌊n/2⌋} (n+1)^{k-1} * binomial(n-1, n-2k)
 *            / k! for n≥1, a(0)=1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required helper utilities */
#include "../utils/factorial.h"   /* seq_t factorial(idx_t n); */
#include "../utils/binomial.h"   /* seq_t binomial(idx_t n, idx_t k); */

seq_t a(idx_t n)
{
    /* Offset 0: first element is 1 */
    if (n == 0)
        return (seq_t)1;

    /* Pre‑compute factorial(n) once */
    seq_t fact_n = factorial(n);

    /* The upper bound of k is ⌊n/2⌋ */
    idx_t max_k = n / 2;
    if (max_k == 0)                /* n = 1 → no summands → 0 */
        return (seq_t)0;

    seq_t sum = 0;
    seq_t P = fact_n;                     /* n! / k!  (k=1 starts with k!=1) */
    seq_t pow_term = 1;                    /* (n+1)^(k-1) , k=1 gives exponent 0 */

    for (idx_t k = 1; k <= max_k; ++k) {
        /* binomial coefficient C(n-1, n-2k) */
        seq_t binom = binomial(n - 1, n - 2 * k);

        /* term = P * binom * pow_term */
        seq_t term = P;
        term *= binom;
        term *= pow_term;
        sum += term;

        /* Prepare for the next iteration if any */
        if (k == max_k)
            break;

        /* Update P: divide by (k+1) to get n!/(k+1)! */
        P /= (seq_t)(k + 1);

        /* Update power term: multiply by (n+1) to get (n+1)^k */
        pow_term *= (seq_t)(n + 1);
    }

    return sum;
}
