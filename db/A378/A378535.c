/*
 * A378535  Möbius transform of A378542, where A378542 is the sum of divisors
 *          d of n such that n/d has an even number of prime factors (counted
 *          with multiplicity).
 * https://oeis.org/A378535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} mobius(d) * A378542(n/d) where A378542(k) =
 *            Sum_{e|k, Ω(k/e) even} e This implements the definition using a
 *            sieve and memoisation
 * Range:     u8 n <= 256, u16 n <= 9721, u32 n <= 9721, u64 n <= 9721,
 *            u128 n <= 9721, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#ifdef __cplusplus
#include <new>
#endif

/* static caches */
static seq_t *f_cache = NULL;   /* A378542 values      */
static seq_t *a_cache = NULL;   /* a(n) values         */
static int   *mu_cache = NULL;  /* Möbius function     */
static uint32_t *spf = NULL;    /* smallest prime factor */
static idx_t  max_n = 0;

/* ------------------------------------------------------------------ */
/* factorisation using previously built SPF table                     */
static size_t factorize_spf(idx_t n, uint64_t *primes, int *exps)
{
    size_t pcnt = 0;
    while (n > 1) {
        uint64_t p = spf[n];
        int e = 0;
        while (n % p == 0) {
            n /= p;
            ++e;
        }
        primes[pcnt] = p;
        exps[pcnt]   = e;
        ++pcnt;
    }
    return pcnt;
}

/* ------------------------------------------------------------------ */
/* compute f(n)=A378542(n) recursively over the divisors of n         */
static void gen_sum_A378542(size_t idx, idx_t cur, int usedOmega,
                            size_t pcnt, const uint64_t *primes,
                            const int *exps, int totalOmega,
                            seq_t *sum)
{
    if (idx == pcnt) {
        if ( ((totalOmega - usedOmega) & 1) == 0 )
            *sum += (seq_t)cur;
        return;
    }
    idx_t val = 1;
    for (int e = 0; e <= exps[idx]; ++e) {
        gen_sum_A378542(idx + 1, cur * val, usedOmega + e,
                        pcnt, primes, exps, totalOmega, sum);
        val *= primes[idx];
    }
}

/* ------------------------------------------------------------------ */
/* compute a(n) via Möbius convolution of f_cache                     */
static void gen_sum_a(size_t idx, idx_t cur, int usedOmega,
                      size_t pcnt, const uint64_t *primes,
                      const int *exps, int totalOmega,
                      idx_t N, seq_t *sum)
{
    if (idx == pcnt) {
        int mu = mu_cache[cur];
        if (mu != 0) {
            seq_t term = f_cache[N / cur];
            if (mu > 0)
                *sum += term;
            else
                *sum -= term;
        }
        return;
    }
    idx_t val = 1;
    for (int e = 0; e <= exps[idx]; ++e) {
        gen_sum_a(idx + 1, cur * val, usedOmega + e,
                  pcnt, primes, exps, totalOmega, N, sum);
        val *= primes[idx];
    }
}

/* ------------------------------------------------------------------ */
/* rebuild caches up to at least N (indifferent of previous size)    */
static void ensure_up_to(idx_t N)
{
    if (N <= max_n) return;

    /* free old caches */
    if (f_cache) free(f_cache);
    if (a_cache) free(a_cache);
    if (mu_cache) free(mu_cache);
    if (spf) free(spf);

    idx_t size = N + 1;
    f_cache  = (seq_t *)calloc((size_t)size, sizeof(seq_t));
    a_cache  = (seq_t *)calloc((size_t)size, sizeof(seq_t));
    mu_cache = (int   *)calloc((size_t)size, sizeof(int));
    spf      = (uint32_t *)calloc((size_t)size, sizeof(uint32_t));
    if (!f_cache || !a_cache || !mu_cache || !spf)
        exit(EXIT_FAILURE);

#ifdef __cplusplus
    /* invoke default constructors for seq_t objects (C++ build only) */
    for (idx_t i = 0; i < size; ++i)
        new (&f_cache[i]) seq_t();
    for (idx_t i = 0; i < size; ++i)
        new (&a_cache[i]) seq_t();
#endif

    /* linear sieve for smallest prime factor */
    uint32_t *prime_list = (uint32_t*)malloc((size_t)size * sizeof(uint32_t));
    size_t prime_cnt = 0;
    for (idx_t i = 2; i <= N; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            prime_list[prime_cnt++] = (uint32_t)i;
        }
        for (size_t j = 0; j < prime_cnt; ++j) {
            uint32_t p = prime_list[j];
            idx_t x = i * (idx_t)p;
            if (x > N) break;
            spf[x] = p;
            if (p == spf[i]) break;
        }
    }
    free(prime_list);

    /* Möbius function */
    mu_cache[1] = 1;
    for (idx_t i = 2; i <= N; ++i) {
        uint32_t p = spf[i];
        idx_t m = i / p;
        if (m % p == 0)
            mu_cache[i] = 0;
        else
            mu_cache[i] = -mu_cache[m];
    }

    /* compute f_cache (A378542) */
    uint64_t primes_local[64];
    int exps_local[64];
    for (idx_t n = 1; n <= N; ++n) {
        size_t pcnt = factorize_spf(n, primes_local, exps_local);
        int totalOmega = 0;
        for (size_t i = 0; i < pcnt; ++i) totalOmega += exps_local[i];
        seq_t s = 0;
        gen_sum_A378542(0, 1, 0, pcnt, primes_local, exps_local,
                        totalOmega, &s);
        f_cache[n] = s;
    }

    max_n = N;
}

/* ------------------------------------------------------------------ */
/* public function required by the problem statement */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n < 1 */
    ensure_up_to(n);
    if (a_cache[n] != (seq_t)0) return a_cache[n];

    uint64_t primes_local[64];
    int exps_local[64];
    size_t pcnt = factorize_spf(n, primes_local, exps_local);
    int totalOmega = 0;
    for (size_t i = 0; i < pcnt; ++i) totalOmega += exps_local[i];

    seq_t sum = 0;
    gen_sum_a(0, 1, 0, pcnt, primes_local, exps_local,
              totalOmega, n, &sum);

    a_cache[n] = sum;
    return sum;
}
