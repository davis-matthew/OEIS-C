/*
 * A359588  Dirichlet inverse of A083346.
 * https://oeis.org/A359588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult, look
 * Formula:   a(n) is the Dirichlet inverse of A083346. A083346 is
 *            multiplicative with A083346(p^e) = p if e mod p ≠ 0,
 *            A083346(p^e) = 1 if e mod p = 0. Hence a(1)=1 and for n>1, a(n)
 *            = -∑_{d|n, d<n} A083346(n/d) * a(d). The function below computes
 *            a(n) multiplicatively from the prime-power decomposition of
 *            n....
 * Range:     i8 n <= 128, i16 n <= 9989, i32 n <= 9989, i64 n <= 9989,
 *            i128 n <= 9989, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 1) return 1;

    idx_t m = n;
    seq_t result = 1;

    /* factorize n, computing the contribution for each prime power */
    for (idx_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            idx_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }

            /* compute Dirichlet inverse for p^exp:
               g[0] = 1, g[i] = -sum_{j=1..i} f(p^j)*g[i-j]        */
            seq_t *g = (seq_t *)malloc((exp + 1) * sizeof(seq_t));
            if (!g) return 0;               /* allocation failed – abort */

            g[0] = 1;
            for (idx_t i = 1; i <= exp; ++i) {
                seq_t sum = 0;
                for (idx_t j = 1; j <= i; ++j) {
                    seq_t f = (j % p == 0) ? 1 : (seq_t)p;
                    sum += f * g[i - j];
                }
                g[i] = -sum;
            }

            result *= g[exp];
            free(g);
        }
    }

    /* any remaining factor is prime > sqrt(original n) */
    if (m > 1) {
        seq_t gprime = -(seq_t)m;   /* f(p) = p, so g(p) = -p */
        result *= gprime;
    }

    return result;
}
