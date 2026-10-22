/*
 * A336278  a(n) = Sum_{k=1..n} mu(k)*k^4.
 * https://oeis.org/A336278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = Σ_{k=1..n} μ(k) * k^4
 * Range:     i8 n <= 4, i16 n <= 12, i32 n <= 173, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation uses a linear sieve to obtain μ(k) up to n
   and sums k^4 weighted by μ(k).  No memoisation is
   required for the expected limits. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;               /* offset starts at 1 */

    idx_t N = n; /* make N a distinct variable for clarity */
    idx_t size = N + 1;

    /* Allocate arrays for the linear sieve */
    int8_t *mu = (int8_t *)calloc(size, sizeof(int8_t));
    bool   *is_comp = (bool *)calloc(size, sizeof(bool));
    idx_t  *primes  = (idx_t *)malloc(size * sizeof(idx_t));
    size_t  prime_cnt = 0;

    if (!mu || !is_comp || !primes)         /* sanity check */
    {
        /* memory allocation failed – return 0 as a safe fault value */
        free(mu); free(is_comp); free(primes);
        return (seq_t)0;
    }

    mu[1] = 1;
    for (idx_t i = 2; i <= N; ++i) {
        if (!is_comp[i]) {
            primes[prime_cnt++] = i;
            mu[i] = -1;                    /* prime contributes -1 to μ */
        }
        for (size_t j = 0; j < prime_cnt; ++j) {
            idx_t p = primes[j];
            idx_t v = p * i;
            if (v > N) break;
            is_comp[v] = true;
            if (i % p == 0) {
                mu[v] = 0;                  /* square factor -> μ(v)=0 */
                break;
            } else {
                mu[v] = -mu[i];
            }
        }
    }

    seq_t sum = 0;                          /* accumulator for a(n) */
    for (idx_t i = 1; i <= N; ++i) {
        seq_t i_seq = (seq_t)i;
        seq_t term  = i_seq * i_seq;        /* i^2   */
        term = term * term;                 /* i^4   */
        term = term * (seq_t)mu[i];         /* μ(i)*i^4 */
        sum += term;
    }

    /* Clean up temporary arrays */
    free(mu);
    free(is_comp);
    free(primes);

    return sum;
}
