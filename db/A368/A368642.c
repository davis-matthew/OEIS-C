/*
 * A368642  a(n) = Sum_{k=1..n} mu(k)^2 * ceiling(n/k), where mu is the Möbius
 *          function (A008683).
 * https://oeis.org/A368642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∑_{k=1..n} μ(k)^2 * ceil(n/k) (OEIS A368642)
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* compute the nth term of the sequence */
seq_t a(idx_t n)
{
    /* guard against the offset; not used but harmless */
    if (n == 0)
        return (seq_t)0;

    /* work with sizes that fit in size_t for array indexing */
    size_t m = (size_t)n + 1;

    /* arrays for the linear sieve */
    uint8_t *mu_sq = (uint8_t *)calloc(m, sizeof(uint8_t));
    uint8_t *is_comp = (uint8_t *)calloc(m, sizeof(uint8_t));
    int *primes = (int *)malloc(m * sizeof(int));
    if (!mu_sq || !is_comp || !primes) {
        free(mu_sq); free(is_comp); free(primes);
        return (seq_t)0;        /* allocation failure – return 0 as fallback */
    }

    /* initial values */
    mu_sq[1] = 1;                /* 1 is square‑free */
    int primes_cnt = 0;

    /* linear sieve for the indicator μ(k)^2 */
    for (idx_t i = 2; i <= n; ++i) {
        if (!is_comp[(size_t)i]) {
            primes[primes_cnt++] = (int)i;
            mu_sq[(size_t)i] = 1;        /* prime is square‑free */
        }
        for (int j = 0; j < primes_cnt; ++j) {
            idx_t p = (idx_t)primes[j];
            idx_t composite = i * p;
            if (composite > n) break;
            is_comp[(size_t)composite] = 1;

            if (i % p == 0) {
                /* p divides i twice ⇒ composite not square‑free */
                mu_sq[(size_t)composite] = 0;
                break;
            } else {
                /* square‑free status is inherited */
                mu_sq[(size_t)composite] = mu_sq[(size_t)i];
            }
        }
    }

    /* compute the sum ∑ μ(k)^2 * ceil(n/k) */
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        if (mu_sq[(size_t)k]) {
            sum += ((seq_t)n + (seq_t)k - 1) / (seq_t)k;  /* ceil(n/k) */
        }
    }

    /* free temporary arrays */
    free(mu_sq);
    free(is_comp);
    free(primes);

    return sum;
}
