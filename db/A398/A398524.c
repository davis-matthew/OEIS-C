/*
 * A398524  a(n) = Sum_{k=1..n} mu(floor(n/k))^2, where mu = A008683 is the
 *          Möbius function.
 * https://oeis.org/A398524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} mu(floor(n/k))^2 where mu is the Möbius
 *            function
 * Range:     u8 n <= 284, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* static workspace for the Möbius sieve */
static int8_t *mu = NULL;              /* μ[1..]  values, -1,0,1 */
static bool *is_comp = NULL;           /* composite flags for sieve */
static uint32_t *primes = NULL;        /* list of primes */
static size_t prime_cnt = 0;           /* number of primes stored */
static idx_t mu_cap = 0;               /* current capacity of mu array */

/* Ensure that μ values are computed up to at least N.
   Recomputes from scratch if N is larger than any previous value. */
static void ensure_mu_up_to(idx_t N)
{
    if (mu_cap >= N + 1) return;          /* already enough */

    /* Reallocate arrays – explicit casts for C++ compatibility */
    mu = (int8_t *)realloc(mu, (N + 1) * sizeof(int8_t));
    is_comp = (bool *)realloc(is_comp, (N + 1) * sizeof(bool));
    primes = (uint32_t *)realloc(primes, N * sizeof(uint32_t));

    /* Linear sieve for μ */
    for (idx_t i = 0; i <= N; ++i) {
        mu[i] = 0;
        is_comp[i] = false;
    }
    prime_cnt = 0;
    mu[1] = 1; /* μ(1) = 1 */

    for (idx_t i = 2; i <= N; ++i) {
        if (!is_comp[i]) {
            primes[prime_cnt++] = (uint32_t)i;
            mu[i] = -1;
        }
        for (size_t j = 0; j < prime_cnt; ++j) {
            idx_t p = primes[j];
            idx_t ip = i * p;
            if (ip > N) break;
            is_comp[ip] = true;
            if (i % p == 0) {
                mu[ip] = 0;
                break;
            } else {
                mu[ip] = -mu[i];
            }
        }
    }
    mu_cap = N + 1;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* out of offset, safe fallback */
    ensure_mu_up_to(n);
    seq_t sum = 0;
    for (idx_t d = 1; d <= n; ++d) {
        if (mu[d] != 0) {                 /* d is square‑free → μ(d)² = 1 */
            idx_t cnt = n / d - n / (d + 1);
            sum += (seq_t)cnt;
        }
    }
    return sum;
}
