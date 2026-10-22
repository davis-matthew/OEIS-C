/*
 * A378991  Dirichlet inverse of the Möbius transform of A005187, where
 *          A005187(n) = 2*n - (number of 1's in binary representation of n).
 * https://oeis.org/A378991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) is the Dirichlet inverse of the Möbius transform of
 *            f(n)=2*n - popcount(n). a(1)=1, for n>1 a(n) = -∑_{d|n, d<n}
 *            g(n/d) * a(d), where g(k)=∑_{d|k} μ(d)*f(k/d)
 * Range:     i8 n <= 41, i16 n <= 1429, i32 n <= 7029, i64 n <= 7029,
 *            i128 n <= 7029, big any n
 * Helpers:   allocate_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/allocate_array.h"

static inline unsigned popcount64(idx_t x) {
    return __builtin_popcountll((unsigned long long)x);
}

static inline seq_t f_val(idx_t k) {
    return (seq_t)2*k - (seq_t)popcount64(k);
}

static inline void free_array(seq_t *p) {
#ifdef __cplusplus
    delete [] p;
#else
    free(p);
#endif
}

static seq_t *a_cache = NULL, *g_cache = NULL, *mu_cache = NULL;
static size_t a_cap = 0;
static idx_t max_comp = 0;

static void compute_up_to(idx_t N) {
    if (N <= max_comp) return;

    /* free old arrays if any */
    if (a_cache)  free_array(a_cache);
    if (g_cache)  free_array(g_cache);
    if (mu_cache) free_array(mu_cache);

    /* allocate new arrays (+1 for 1-based indexing) */
    a_cache = allocate_array(N + 1);
    g_cache = allocate_array(N + 1);
    mu_cache = allocate_array(N + 1);

    /* small auxiliary arrays for sieve */
    unsigned char *is_comp = (unsigned char*)calloc(N + 1, sizeof(unsigned char));
    idx_t *primes = (idx_t*)malloc((N + 1) * sizeof(idx_t));
    size_t pcnt = 0;

    /* linear sieve for Möbius function */
    mu_cache[1] = (seq_t)1;
    for (idx_t i = 2; i <= N; ++i) {
        if (!is_comp[i]) {
            primes[pcnt++] = i;
            mu_cache[i] = (seq_t)-1;
        }
        for (size_t j = 0; j < pcnt; ++j) {
            idx_t p = primes[j];
            idx_t np = i * p;
            if (np > N) break;
            is_comp[np] = 1;
            if (i % p == 0) {
                mu_cache[np] = (seq_t)0;
                break;
            } else {
                mu_cache[np] = -mu_cache[i];
            }
        }
    }

    free(is_comp);

    /* compute g[n] = sum_{d|n} mu(d)*f(n/d) */
    for (idx_t i = 0; i <= N; ++i) g_cache[i] = (seq_t)0;
    for (idx_t d = 1; d <= N; ++d) {
        seq_t mu_d = mu_cache[d];
        if (!mu_d) continue;
        for (idx_t m = d; m <= N; m += d) {
            idx_t k = m / d;
            g_cache[m] += mu_d * f_val(k);
        }
    }

    /* compute a[n] via Dirichlet inverse recurrence */
    a_cache[0] = (seq_t)0;        /* unused, but defined for safety */
    a_cache[1] = (seq_t)1;
    for (idx_t n = 2; n <= N; ++n) {
        seq_t sum = (seq_t)0;
        for (idx_t d = 1; d*d <= n; ++d) {
            if (n % d != 0) continue;
            idx_t d1 = d;
            idx_t d2 = n / d;
            if (d1 < n) sum += g_cache[d2] * a_cache[d1];
            if (d2 != d1 && d2 < n) sum += g_cache[d1] * a_cache[d2];
        }
        a_cache[n] = -sum;
    }

    max_comp = N;
    free(primes);
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* out of range guard */
    compute_up_to(n);
    return a_cache[n];
}
