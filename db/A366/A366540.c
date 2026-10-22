/*
 * A366540  Number of commutative binary operators defined on the finite chain
 *          L_n={0,1,...n}, C:L_n^2-> L_n, which are increasing in each
 *          argument, and satisfy the boundary conditions C(0,n)=C(n,0)=0 and
 *          C(n,n)=n.
 * https://oeis.org/A366540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = \prod_{i=1..n}\prod_{j=i..n}\frac{i+j+n-1}{i+j-1} \;
 *            \;\;-\; \prod_{i=1..n}\prod_{j=i..n}\frac{i+j+n-2}{i+j-1}
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses the prime‑factorisation method described
   in the OEIS entry.  It requires the utilities seq_mul() and seq_pow()
   for arbitrary‑precision arithmetic.  The code is valid C11 and also
   compiles in C++17. */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

/* --- Prime sieve and factorisation tables -------------------------------- */
#define MAX_SPF_LIMIT 60000               /* covers n up to about 20000 safely */
static int spf[MAX_SPF_LIMIT + 1];       /* smallest prime factor table   */
static int prime_list[6000];             /* store primes up to MAX_SPF_LIMIT */
static size_t prime_count = 0;
static int current_spf_limit = 0;

/* Initialise smallest‑prime‑factor table and prime list. */
static void init_spf(int limit) {
    if (limit > MAX_SPF_LIMIT) limit = MAX_SPF_LIMIT;
    for (int i = 0; i <= limit; ++i) spf[i] = i;
    for (int i = 2; i * i <= limit; ++i) {
        if (spf[i] == i) {
            for (int j = i * i; j <= limit; j += i)
                if (spf[j] == j) spf[j] = i;
        }
    }
    prime_count = 0;
    for (int i = 2; i <= limit; ++i)
        if (spf[i] == i) prime_list[prime_count++] = i;
    current_spf_limit = limit;
}

/* Ensure tables are built up to required limit. */
static void ensure_spf(int limit) {
    if (current_spf_limit < limit) init_spf(limit);
}

/* Helper for accumulating prime exponents of a number. */
static void fact_add(int value, int times, int sign, int *exp, int limit) {
    while (value > 1) {
        int p = spf[value];
        int cnt = 0;
        while (value % p == 0) { value /= p; cnt++; }
        exp[p] += sign * times * cnt;
    }
}

/* Compute product for D(n): \prod_{i<=j} (i+j+n-1)/(i+j-1). */
static seq_t compute_D(idx_t n) {
    int nn = (int)n;
    int limit = 3 * nn;
    if (limit > MAX_SPF_LIMIT) limit = MAX_SPF_LIMIT;
    ensure_spf(limit);

    static int exp[MAX_SPF_LIMIT + 1];
    memset(exp, 0, (limit + 1) * sizeof(int));

    for (int t = 2; t <= 2 * nn; ++t) {
        int f;
        if (t <= nn + 1)
            f = t / 2;
        else
            f = t / 2 - t + nn + 1;
        if (f == 0) continue;
        int num = t + nn - 1;
        int den = t - 1;
        fact_add(num, f, +1, exp, limit);
        fact_add(den, f, -1, exp, limit);
    }

    seq_t res = 1;
    for (size_t i = 0; i < prime_count && prime_list[i] <= limit; ++i) {
        int p = prime_list[i];
        int e = exp[p];
        if (e > 0) {
            seq_t pw = seq_pow((seq_t)p, (idx_t)e);
            res = seq_mul(res, pw);
        }
    }
    return res;
}

/* Compute product for E(n): \prod_{i<=j} (i+j+n-2)/(i+j-1). */
static seq_t compute_E(idx_t n) {
    int nn = (int)n;
    int limit = 3 * nn;
    if (limit > MAX_SPF_LIMIT) limit = MAX_SPF_LIMIT;
    ensure_spf(limit);

    static int exp[MAX_SPF_LIMIT + 1];
    memset(exp, 0, (limit + 1) * sizeof(int));

    for (int t = 2; t <= 2 * nn; ++t) {
        int f;
        if (t <= nn + 1)
            f = t / 2;
        else
            f = t / 2 - t + nn + 1;
        if (f == 0) continue;
        int num = t + nn - 2;
        int den = t - 1;
        fact_add(num, f, +1, exp, limit);
        fact_add(den, f, -1, exp, limit);
    }

    seq_t res = 1;
    for (size_t i = 0; i < prime_count && prime_list[i] <= limit; ++i) {
        int p = prime_list[i];
        int e = exp[p];
        if (e > 0) {
            seq_t pw = seq_pow((seq_t)p, (idx_t)e);
            res = seq_mul(res, pw);
        }
    }
    return res;
}

/* --- Memoised sequence a(n) --------------------------------------------- */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static idx_t cached_max = 0;

seq_t a(idx_t n) {
    if (n == 0) return 0;                      /* offset starts at 1 */

    /* Allocate or grow cache as needed */
    if (cache == NULL || cache_size < (size_t)n + 1) {
        seq_t *new_cache = (seq_t*)malloc(sizeof(seq_t) * ((size_t)n + 1));
        if (cache != NULL) {
            memcpy(new_cache, cache, sizeof(seq_t) * cache_size);
            free(cache);
        }
        cache = new_cache;
        cache_size = (size_t)n + 1;
    }

    /* Compute missing entries */
    for (idx_t k = cached_max + 1; k <= n; ++k) {
        seq_t d = compute_D(k);
        seq_t e = compute_E(k);
        cache[k] = d - e;                     /* safe: d >= e for all k */
    }
    cached_max = n;
    return cache[n];
}
