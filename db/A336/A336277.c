/*
 * A336277  a(n) = Sum_{k=1..n} mu(k)*k^3.
 * https://oeis.org/A336277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = Sum_{k=1..n} μ(k) * k^3
 * Range:     i8 n <= 4, i16 n <= 29, i32 n <= 653, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static idx_t *spf_cache = NULL;      /* smallest prime factor array */
static idx_t spf_limit  = 0;         /* current maximum index in spf_cache */

/* Compute smallest prime factors for 1..limit (inclusive). */
static void compute_spf(idx_t limit)
{
    idx_t *new_spf = (idx_t*)malloc((limit + 1) * sizeof(idx_t));
    new_spf[0] = 0;
    if(limit >= 1) new_spf[1] = 1;
    for (idx_t i = 2; i <= limit; ++i) new_spf[i] = 0;

    for (idx_t i = 2; i * i <= limit; ++i) {
        if (new_spf[i] == 0) {                /* i is prime */
            for (idx_t j = i * i; j <= limit; j += i) {
                if (new_spf[j] == 0) new_spf[j] = i;
            }
        }
    }
    for (idx_t i = 2; i <= limit; ++i)
        if (new_spf[i] == 0) new_spf[i] = i;   /* primes */

    free(spf_cache);
    spf_cache = new_spf;
    spf_limit = limit;
}

/* Ensure spf_cache contains values up to needed. */
static void ensure_spf(idx_t needed)
{
    if (needed > spf_limit) compute_spf(needed);
}

/* Compute μ(k) using the precomputed smallest prime factors. */
static inline int8_t mu_of_k(idx_t k)
{
    if (k <= 1) return 1;
    int8_t mu = 1;
    idx_t prev_p = 0;
    while (k > 1) {
        idx_t p = spf_cache[k];
        if (p == prev_p) return 0;   /* squared prime factor detected */
        prev_p = p;
        mu = -mu;
        k /= p;
    }
    return mu;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* defined for n ≥ 1 by offset */

    ensure_spf(n);

    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        int8_t mu_k = mu_of_k(k);
        if (mu_k == 0) continue;
        seq_t kk = (seq_t)k;
        seq_t cube = kk * kk * kk;
        sum += (mu_k > 0) ? cube : -cube;
    }
    return sum;
}
