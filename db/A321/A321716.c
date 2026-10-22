/*
 * A321716  Triangle read by rows: T(n,k) is the number of n X k Young
 *          tableaux, where 0 <= k <= n.
 * https://oeis.org/A321716
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = (n*k)! / ∏_{i=1..n} ∏_{j=1..k} (i+j-1) (Young tableaux
 *            count)
 * Range:     u8 n <= 12, u16 n <= 18, u32 n <= 25, u64 n <= 34, u128 n <= 52,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/seq_pow.h"

#define MAX_NK   20000        /* maximum value of n*k (≈141*141) */
#define MAX_PRIMES 2500       /* ample space for primes up to MAX_NK */

/* global tables */
static int primes[MAX_PRIMES];
static int prime_cnt = 0;
static int spf[MAX_NK + 1];
static int prime_index[MAX_NK + 1] = {0};

/* sieve for primes and smallest prime factors */
static void init_primes(void)
{
    if (prime_cnt != 0)
        return;

    for (int i = 2; i <= MAX_NK; ++i)
        spf[i] = i;

    for (int i = 2; i * i <= MAX_NK; ++i) {
        if (spf[i] == i) {
            for (int j = i * i; j <= MAX_NK; j += i)
                if (spf[j] == j)
                    spf[j] = i;
        }
    }

    for (int i = 2; i <= MAX_NK; ++i) {
        if (spf[i] == i) {          /* i is prime */
            prime_index[i] = prime_cnt;
            primes[prime_cnt++] = i;
        }
    }
    /* mark all non‑prime indices with -1 */
    for (int i = 2; i <= MAX_NK; ++i)
        if (prime_index[i] == 0 && spf[i] != i)
            prime_index[i] = -1;
}

/* Legendre exponent of prime p in m! */
static inline int legendre_exp(idx_t m, int p)
{
    int e = 0;
    while (m) {
        m /= p;
        e += (int)m;
    }
    return e;
}

/* compute T(n,k) for 0 <= k <= n */
static seq_t compute_T(idx_t n, idx_t k)
{
    if (n == 0 || k == 0)
        return (seq_t)1;

    init_primes();

    static int exp[MAX_PRIMES];
    /* zero exponents */
    for (int i = 0; i < prime_cnt; ++i)
        exp[i] = 0;

    /* numerator: (n*k)! */
    idx_t nk = n * k;
    for (int pi = 0; pi < prime_cnt; ++pi) {
        int p = primes[pi];
        if ((idx_t)p > nk)
            break;
        exp[pi] = legendre_exp(nk, p);
    }

    /* denominator: ∏_{i=1..n}∏_{j=1..k} (i+j-1) */
    int max_t = (int)(n + k - 1);
    for (int t = 2; t <= max_t; ++t) {
        /* multiplicity of t in the denominator */
        int lower = t - (int)k + 1;
        if (lower < 1)
            lower = 1;
        int upper = (t < (int)n) ? t : (int)n;
        if (upper < lower)
            continue;
        int m = upper - lower + 1;         /* multiplicity */

        /* factorise t */
        int x = t;
        while (x > 1) {
            int f = spf[x];
            int cnt = 0;
            while (x % f == 0) {
                x /= f;
                ++cnt;
            }
            int pi = prime_index[f];
            exp[pi] -= cnt * m;
        }
    }

    /* assemble the result from prime exponents */
    seq_t val = (seq_t)1;
    for (int pi = 0; pi < prime_cnt; ++pi) {
        int e = exp[pi];
        if (e > 0)
            val = val * seq_pow((seq_t)primes[pi], (idx_t)e);
    }
    return val;
}

/* cache for all computed a(n) */
static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* helper to grow cache */
static void ensure_cache_size(size_t need)
{
    if (cache_cap >= need)
        return;
    size_t new_cap = cache_cap ? cache_cap : 8;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_cache;
    cache_cap = new_cap;
}

/* state for incremental construction */
static idx_t next_index = 0;    /* next sequence index to compute */
static idx_t cur_row = 0;
static idx_t cur_col = 0;

/* public sequence function */
seq_t a(idx_t n)
{
    if (n >= cache_cap)
        ensure_cache_size(n + 1);

    while (next_index <= n) {
        seq_t val = compute_T(cur_row, cur_col);
        cache[next_index++] = val;

        /* advance to next cell in the triangle */
        if (cur_col == cur_row) {
            ++cur_row;
            cur_col = 0;
        } else {
            ++cur_col;
        }
    }
    return cache[n];
}
