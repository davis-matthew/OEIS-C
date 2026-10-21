/*
 * A277423  a(n) = n!*LaguerreL(n, n).
 * https://oeis.org/A277423
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n!*LaguerreL(n, n) = Sum_{k=0..n} (-n)^(n-k) * k! *
 *            binomial(n,k)^2 (OTTE: OEIS A277423)
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 11, i64 n <= 18, i128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* external utilities; their prototypes are already provided elsewhere */
seq_t factorial(idx_t n);
seq_t binomial(idx_t n, idx_t k);

/* compute a(n) for any n >= 0, returning seq_t */
seq_t a(idx_t n)
{
    if (n == 0) return 1;

    seq_t sum = 0;
    seq_t fact = 1;        /* k!   (k starts at 0) */
    seq_t binom = 1;       /* C(n,0) = 1 */

    /* pre‑compute powers of n: pow[e] = n^e, e = 0..n */
    seq_t *pow = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    if (!pow) return 0;    /* allocation failure – fallback to 0 */
    pow[0] = 1;
    for (idx_t e = 1; e <= n; ++e)
        pow[e] = pow[e - 1] * (seq_t)n;

    for (idx_t k = 0; k <= n; ++k)
    {
        idx_t exp = n - k;                     /* exponent for (-n) */
        seq_t term = fact * binom;
        term = term * binom;                   /* = fact * binom^2 */
        term = term * pow[exp];                /* multiply by n^exp */

        if (exp & 1)                           /* apply sign (-1)^exp */
            term = -term;

        sum = sum + term;

        if (k < n) {                           /* update for next k */
            fact = fact * (k + 1);              /* (k+1)!   */
            /* binom(n,k+1) = binom(n,k)*(n-k)/(k+1) */
            binom = binom * (n - k) / (k + 1);
        }
    }

    free(pow);
    return sum;
}
