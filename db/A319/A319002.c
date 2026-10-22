/*
 * A319002  Number of ordered factorizations of n where the sequence of GCDs
 *          of prime indices (A289508) of the factors is weakly increasing.
 * https://oeis.org/A319002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered factorizations of n whose factors *
 *            have weakly increasing GCDs of the prime indices of the factors
 *            * (OEIS A319002)
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   build_primes_upto, build_spf, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* utility headers provided by the build */
#include "../utils/gcd.h"
#include "../utils/build_primes_upto.h"
#include "../utils/build_spf.h"

static seq_t *dp          = NULL;  /* memoization table */
static size_t dp_size     = 0;     /* number of entries in dp */
static idx_t max_n        = 0;     /* largest n that has been processed */

static uint16_t *prime_index = NULL; /* prime_index[p] = 1‑based index of prime p */
static size_t   prime_cnt    = 0;     /* number of primes <= max_n */

static idx_t *spf = NULL;            /* smallest prime factor for each number */
static uint16_t *g_of = NULL;        /* g_of[n] = GCD of prime indices of n (0 for n<=1) */

/* --------------------------------------------------------------------- */
/* Create all tables needed for numbers up to/up_to.                    */
/* All previous tables are freed and rebuilt when up_to > max_n.         */
static void precompute(idx_t up_to)
{
    if (up_to <= max_n)
        return;

    /* free existing data */
    free(dp);
    free(prime_index);
    free(spf);
    free(g_of);
    dp       = NULL;
    dp_size  = 0;
    prime_cnt= 0;
    prime_index = NULL;
    spf          = NULL;
    g_of         = NULL;

    /* ----------------- build primes up to up_to --------------------- */
    idx_t *pr = NULL;
    size_t pr_cnt = 0;
    build_primes_upto(up_to, &pr, &pr_cnt);

    prime_index = (uint16_t*)calloc(up_to + 1, sizeof(uint16_t));
    for (size_t i = 0; i < pr_cnt; ++i)
        prime_index[pr[i]] = (uint16_t)(i + 1); /* 1‑based */
    free(pr);
    prime_cnt = pr_cnt;

    /* -------------- build smallest prime factor table --------------- */
    build_spf(up_to, &spf);

    /* -------------- compute g_of for every n with n>=2 --------------- */
    g_of = (uint16_t*)calloc(up_to + 1, sizeof(uint16_t));
    for (idx_t i = 2; i <= up_to; ++i) {
        idx_t x = i;
        uint16_t g = 0;
        while (x > 1) {
            idx_t p = spf[x];
            uint16_t idxp = prime_index[p];
            if (g == 0)
                g = idxp;
            else
                g = (uint16_t)gcd((seq_t)g, (seq_t)idxp);
            while (x % p == 0)
                x /= p;
        }
        g_of[i] = g;
    }

    /* ----------------- allocate memoization table ----------------- */
    dp_size = (up_to + 1) * (prime_cnt + 1);
    dp = (seq_t*)calloc(dp_size, sizeof(seq_t));   /* zeros */

    max_n = up_to;
}

/* --------------------------------------------------------------------- */
/* Recursively count factorizations of n with GCDs >= min_g in order. */
static seq_t solve(idx_t n, uint16_t min_g)
{
    if (n == 1)
        return 1;       /* empty factorization */

    size_t key = (size_t)n * (prime_cnt + 1) + min_g;
    seq_t cached = dp[key];
    if (cached != 0)
        return cached;

    seq_t result = 0;

    /* factor n using spf */
    idx_t temp = n;
    idx_t *fac_primes = NULL;
    int *fac_exps    = NULL;
    size_t fac_cnt   = 0;

    while (temp > 1) {
        idx_t p = spf[temp];
        int e = 0;
        while (temp % p == 0) {
            temp /= p;
            ++e;
        }
        fac_primes = (idx_t*)realloc(fac_primes, (fac_cnt + 1) * sizeof(idx_t));
        fac_exps   = (int*)realloc(fac_exps,   (fac_cnt + 1) * sizeof(int));
        fac_primes[fac_cnt] = p;
        fac_exps[fac_cnt]   = e;
        ++fac_cnt;
    }

    /* generate all divisors of n */
    size_t divs_sz = 1;
    idx_t *divs = (idx_t*)malloc(sizeof(idx_t));
    divs[0] = 1;

    for (size_t i = 0; i < fac_cnt; ++i) {
        idx_t p    = fac_primes[i];
        int   e    = fac_exps[i];
        size_t old_sz = divs_sz;
        size_t new_sz = divs_sz * (e + 1);

        divs = (idx_t*)realloc(divs, new_sz * sizeof(idx_t));

        idx_t pow = 1;
        for (int exp = 1; exp <= e; ++exp) {
            pow *= p;
            for (size_t j = 0; j < old_sz; ++j) {
                divs[divs_sz++] = divs[j] * pow;
            }
        }
    }

    /* iterate over divisors > 1 */
    for (size_t i = 0; i < divs_sz; ++i) {
        idx_t d = divs[i];
        if (d <= 1)
            continue;
        uint16_t gd = g_of[d];
        if (gd >= min_g) {
            result += solve(n / d, gd);
        }
    }

    free(divs);
    free(fac_primes);
    free(fac_exps);

    dp[key] = result;
    return result;
}

/* --------------------------------------------------------------------- */
/* Public entry point required by the evaluator. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;
    precompute(n);
    return solve(n, 0);      /* no constraint on the first factor */
}
