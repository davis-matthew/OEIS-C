/*
 * A351834  Number of partitions of n into exactly two distinct parts, at
 *          least one of which is squarefree.
 * https://oeis.org/A351834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..⌊(n-1)/2⌋} (mu(k)^2 + mu(n‑k)^2 −
 *            mu(k)^2*mu(n‑k)^2) * i.e. the number of k in 1..⌊(n‑1)/2⌋ for
 *            which exactly one of k and n‑k is * square‑free
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/* Linear sieve that fills an array with the Möbius function.              */
/* mu_cache[0] is unused; mu_cache[1] = 1;                                 */
/* --------------------------------------------------------------------- */

static int8_t *mu_cache = NULL;          /* cache of mu values   */
static size_t mu_cap   = 0;              /* current allocated size */

/* Ensure the cache contains values up to at least ‘limit’.             */
static void ensure_mu(idx_t limit)
{
    if (limit < mu_cap)
        return;

    /* Allocate a larger array. */
    size_t new_cap = (size_t)limit + 1;
    int8_t *tmp = (int8_t *)realloc(mu_cache, new_cap * sizeof(int8_t));
    if (!tmp)
        abort();               /* memory exhaustion */
    mu_cache = tmp;

    /* Seive: we need a list of primes and a flag array.                */
    idx_t *primes   = (idx_t *)malloc(new_cap   * sizeof(idx_t));
    idx_t *is_comp  = (idx_t *)calloc(new_cap,  sizeof(idx_t));
    size_t pcnt     = 0;

    /* Initialise first entries. */
    if (new_cap > 0) mu_cache[0] = 0;
    if (new_cap > 1) mu_cache[1] = 1;

    for (idx_t i = 2; i < new_cap; ++i) {
        if (!is_comp[i]) {          /* i is prime */
            primes[pcnt++] = i;
            mu_cache[i]   = -1;     /* μ(prime) = –1 */
        }
        for (size_t j = 0; j < pcnt && i * primes[j] < new_cap; ++j) {
            idx_t v = i * primes[j];
            is_comp[v] = 1;
            if (i % primes[j] == 0) {
                mu_cache[v] = 0;     /* square factor */
                break;
            } else {
                mu_cache[v] = (int8_t)(-mu_cache[i]); /* μ(ab)=μ(a)·μ(b) */
            }
        }
    }

    free(primes);
    free(is_comp);
    mu_cap = new_cap;
}

/* Return the Möbius value μ(n).  μ(0) is defined as 0 for convenience. */
static int get_mu(idx_t n)
{
    if (n == 0)
        return 0;
    ensure_mu(n);
    return (int)mu_cache[n];
}

/* --------------------------------------------------------------------- */
/* Public function required by the build system.                          */
/* --------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    /* For n < 1 the function is unconstrained, but the spec requires n ≥ 1. */
    if (n <= 1)
        return (seq_t)0;

    idx_t limit = (n - 1) / 2;          /* floor((n-1)/2) */
    seq_t total = 0;

    for (idx_t k = 1; k <= limit; ++k) {
        int mu_k  = get_mu(k);
        int mu_nk = get_mu(n - k);

        /* μ(k) != 0  <=>  μ(k)^2 = 1  <=>  k is square‑free   */
        seq_t t1 = mu_k  ? (seq_t)1 : (seq_t)0;
        seq_t t2 = mu_nk ? (seq_t)1 : (seq_t)0;

        /* contribution: 1 if exactly one of k and n‑k is square‑free */
        seq_t tsum = t1 + t2 - ((t1 & t2));
        total += tsum;
    }

    return total;
}
