/*
 * A328711  Dirichlet g.f.: (1/2) * Product_{i>=1, j>=1} (1 + (i*j)^(-s))^i.
 * https://oeis.org/A328711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of n in ∏_{k≥2} (1 + k^{-s})^{σ(k)} , where
 *            σ(k)=sum of divisors of k. a(n) = Σ_{n = ∏ k^{j_k}} ∏ C(σ(k),
 *            j_k) (summed over all factorizations of n into powers of
 *            distinct integers k≥2)
 * Range:     u8 n <= 23, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdint.h>   /* uint64_t */
#include <stdlib.h>   /* malloc, free */

/* binom(a,b) is provided in the utils. */
#include "../utils/binom.h"

/* cache to allow repeated calls */
static seq_t *seq_cache = NULL;     /* array of a(0..max_n) */
static idx_t  cache_max  = 0;       /* maximum n computed */

/* Compute the sequence up to given N (N>=1) and store in seq_cache. */
static void compute_up_to(idx_t N)
{
    /* Allocate new buffer for the results (size N+1). */
    seq_t *new_cache = (seq_t*)malloc((N+1) * sizeof(seq_t));
    if (!new_cache) return;          /* allocation failed */

    /* Initialise all entries to 0; a(1)=1 */
    for (size_t i = 0; i < (size_t)(N+1); ++i) new_cache[i] = 0;
    new_cache[1] = 1;

    /* tmp buffer for convolution */
    seq_t *tmp = (seq_t*)malloc((N+1) * sizeof(seq_t));
    if (!tmp) { free(new_cache); return; }

    /* Pre‑compute σ(k) for k=1..N (sum of divisors) */
    idx_t *sigma = (idx_t*)malloc((N+1) * sizeof(idx_t));
    if (!sigma) { free(tmp); free(new_cache); return; }
    for (idx_t i = 0; i <= N; ++i) sigma[i] = 0;
    for (idx_t i = 1; i <= N; ++i)
        for (idx_t j = i; j <= N; j += i)
            sigma[j] += i;          /* σ(j) += i */

    /* Iterate over each base k=2..N and perform Dirichlet convolution */
    for (idx_t k = 2; k <= N; ++k)
    {
        /* Copy current cache to tmp (will become new cache) */
        seq_t *old = new_cache;
        seq_t *next = tmp;
        for (size_t i = 0; i <= (size_t)N; ++i) next[i] = old[i];

        /* Compute powers of k and add contributions */
        idx_t power = k;
        idx_t e = 1;
        while (power <= N) {
            seq_t weight = binom(sigma[k], e);          /* C(σ(k), e) */
            /* For every multiple n = power * m  (m≥1), add contribution
               from m using the old cache before this base was processed. */
            for (idx_t m = 1, n = power; n <= N; ++m, n += power) {
                next[n] += old[m] * weight;
            }
            /* Advance to next exponent */
            if (power > N / k) break;   /* prevents overflow */
            power *= k;
            ++e;
        }

        /* Swap pointers: new_cache now holds the updated values */
        new_cache = next;
        tmp     = old;            /* old becomes tmp for next iteration */
    }

    /* Free temporary buffers and old cache */
    free(sigma);
    free(tmp);           /* tmp now holds the old cache to be freed */
    if (seq_cache) free(seq_cache);
    seq_cache = new_cache;
    cache_max = N;
}

/* Public function required by the framework. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* outside of defined range */
    if (n > cache_max) compute_up_to(n);     /* ensure cache covers n */
    return seq_cache[n];
}
