/*
 * A292768  Partial sums of A065387.
 * https://oeis.org/A292768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 240, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A292768: Partial sums of A065387
   a(n) = sum_{d=1..n} (mu(d)+1) * (M*(M+1)/2)  with M = floor(n/d)
   where mu is the Möbius function.
   This implements the formula derived from the GF in the OEIS entry. */

#include <stdlib.h>
#include <stdint.h>

/* Compute the sequence value for index n. */
seq_t a(idx_t n)
{
    /* Use a static cache for mu up to the largest n seen. */
    static idx_t max_n = 0;
    static int8_t *mu = NULL;   /* mu[1..max_n], 1-indexed */

    /* Expand the cache if necessary. */
    if (n > max_n) {
        size_t new_n = (size_t)n + 1;            /* array size = n+1 */
        int8_t *new_mu = (int8_t *)calloc(new_n, sizeof(int8_t));
        int    *lp      = (int    *)calloc(new_n, sizeof(int));
        int    *pr      = (int    *)malloc (new_n * sizeof(int));
        if (!new_mu || !lp || !pr) abort();

        int pc = 0;                      /* number of primes found */
        new_mu[1] = 1;                   /* mu(1) = 1 */

        for (idx_t i = 2; i <= n; ++i) {
            idx_t ui = i;
            if (lp[ui] == 0) {           /* i is prime */
                lp[ui] = (int)ui;
                pr[pc++] = (int)ui;
                new_mu[ui] = -1;         /* mu(i) = -1 for a prime */
            }
            for (int k = 0; k < pc; ++k) {
                int p = pr[k];
                size_t prod = (size_t)ui * (size_t)p;
                if (prod > (size_t)n) break;
                lp[prod] = p;
                if (i % p == 0) {         /* repeated prime factor */
                    new_mu[prod] = 0;
                    break;
                } else {
                    new_mu[prod] = -new_mu[ui];
                }
            }
        }

        /* Replace old cache. */
        free(mu);
        free(lp);
        free(pr);
        mu     = new_mu;
        max_n  = n;
    }

    /* Compute the partial sum. */
    seq_t result = 0;
    for (idx_t d = 1; d <= n; ++d) {
        int mu_val = (int)mu[d];             /* mu(d) ∈ {-1,0,1} */
        int mu_plus = mu_val + 1;             /* 0, 1, or 2 */
        if (mu_plus == 0) continue;

        idx_t M = n / d;                      /* floor(n/d) */
        seq_t m = (seq_t)M;
        seq_t t = m * (m + 1) / 2;           /* m*(m+1)/2 */

        result += (seq_t)mu_plus * t;
    }
    return result;
}
