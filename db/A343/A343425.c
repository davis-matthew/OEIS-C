/*
 * A343425  a(n) = Sum_{k=1..n} mu(k) * n^(n - k).
 * https://oeis.org/A343425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} mu(k) * n^(n-k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_small.h"

static idx_t     mu_cap   = 0;          /* largest n computed for μ */
static int      *mu       = NULL;      /* mu[1..n] : -1,0,1 */
static uint64_t *primes   = NULL;      /* prime list for the sieve */
static size_t    primes_sz = 0;        /* number of primes found */

/* Recompute Möbius function up to n (inclusive) */
static void ensure_mu(idx_t n)
{
    if (n <= mu_cap)                 /* already computed */
        return;

    /* free previous data, if any */
    free(mu);
    free(primes);

    /* allocation – cast from void* to the required pointer type */
    mu = (int *)calloc((size_t)(n + 1), sizeof(int));
    if (!mu)                          /* allocation failure */
        return;

    primes = (uint64_t *)malloc((size_t)n * sizeof(uint64_t));
    if (!primes) {
        free(mu);
        mu = NULL;
        return;
    }

    primes_sz = 0;
    mu[1] = 1;

    for (idx_t i = 2; i <= n; ++i) {
        /* i is prime ? */
        if (is_prime_small((seq_t)i)) {
            primes[primes_sz++] = i;
            mu[i] = -1;
        }

        for (size_t j = 0; j < primes_sz; ++j) {
            uint64_t p = primes[j];
            idx_t  x   = i * p;
            if (x > n)              /* beyond limit */
                break;

            if (i % p == 0) {       /* p divides i -> not square‑free */
                mu[x] = 0;
                break;
            } else {
                mu[x] = -mu[i];
            }
        }
    }

    mu_cap = n;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* a(0) is undefined but return 0 */

    ensure_mu(n);

    seq_t sum   = 0;
    seq_t power = 1;                 /* n^(n-n) = n^0 */

    for (idx_t k = n; k >= 1; --k) {
        int m = mu[k];
        if (m != 0) {
            if (m > 0)
                sum += power;
            else
                sum -= power;
        }
        if (k > 1)                     /* prepare next power: power *= n */
            power *= (seq_t)n;
    }
    return sum;
}
