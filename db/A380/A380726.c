/*
 * A380726  E.g.f. A(x) satisfies A(x) = exp(x * A(x)^2 / (1 - x*A(x)^2)) / (1
 *          - x*A(x)^2).
 * https://oeis.org/A380726
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   binom_3n_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A380726
 * Formula: a(n) = n! * Sum_{k=0..n} (2*n+1)^(k-1) * binomial(3*n, n-k)/k!
 * (OEIS offset 0)
 * Implementation uses only integer arithmetic on the seq_t type.
 */
#include <stdint.h> /* for uint64_t used by idx_t */
#include "../utils/binom_3n_n.h"

static seq_t factorial(idx_t n) {
    seq_t f = 1;
    for (idx_t i = 2; i <= n; ++i)
        f *= (seq_t)i;
    return f;
}

seq_t a(idx_t n) {
    if (n == 0)
        return 1; /* a(0) = 1 */

    seq_t fact_n = factorial(n);
    seq_t base   = (seq_t)(2 * n + 1);
    seq_t c      = binom_3n_n(n);              /* binom(3n, n)   */
    seq_t sum    = (c * fact_n) / base;       /* term for k = 0 */

    /* Prepare for k = 1 */
    idx_t r = n;                              /* current r = n-k */
    /* c = binom(3n, n-1) using recurrence */
    c = c * (seq_t)r / ( (seq_t)(3 * n) - (seq_t)r + 1 );
    r--;                                     /* now r = n-1   */
    seq_t prod   = fact_n;                    /* n! / k!  (k=0)  */
    seq_t power  = 1;                         /* base^(k-1) for k=1 */

    for (idx_t k = 1; k <= n; ++k) {
        /* term for current k */
        seq_t term = c * prod * power;
        sum += term;

        if (k < n) {                          /* prepare next iteration */
            prod = prod / (seq_t)(k + 1);     /* n! / (k+1)!   */

            /* update binomial for next r (r-1) */
            if (r > 0)
                c = c * (seq_t)r / ( (seq_t)(3 * n) - (seq_t)r + 1 );
            else
                c = 1;                         /* binom(3n,0) = 1 */

            r--;                              /* next r value   */
            power = power * base;             /* base^(k) for next k+1 */
        }
    }
    return sum;
}
