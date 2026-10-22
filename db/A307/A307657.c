/*
 * A307657  G.f. A(x) satisfies: 1/(1 + x) = A(x)*A(x^2)^2*A(x^3)^3*A(x^4)^4*
 *          ... *A(x^k)^k* ...
 * https://oeis.org/A307657
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   1/(1 + x) = A(x) * ∏_{k>=2} A(x^k)^k. Using logarithms this
 *            gives the recurrences F(1) = -1 F(n) = (-1)^n − ∑_{d|n, d≥2} d²
 *            · F(n/d) (F(n) = n · f(n)) a(0) = 1 a(n) = (1/n) · ∑_{k=1}^n
 *            F(k) · a(n−k) (a(n) is the term of A(x))
 * Range:     i8 n <= 6, i16 n <= 25, i32 n <= 47, i64 n <= 47, i128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* ------------------------------------------------------------------ */
/*  Möbius‐function sieve (linear) -----------------------------------*/
static int *mu_cache      = NULL;   /* Möbius values (mu[0] unused) */
static idx_t *spf_cache   = NULL;   /* smallest prime factor, not needed after the sieve */
static idx_t *primes_cache= NULL;   /* list of primes during the sieve */
static size_t mu_cap      = 0;
static size_t primes_cnt  = 0;

/* Extend the sieve so that μ(i) is known for all 0 ≤ i ≤ N. */
static void init_mu(idx_t N)
{
    if (N < mu_cap) return;                     /* already up to date */
    size_t new_cap = N + 1;
    mu_cache      = (int *)realloc(mu_cache,      new_cap * sizeof(int));
    spf_cache     = (idx_t *)realloc(spf_cache,  new_cap * sizeof(idx_t));
    primes_cache  = (idx_t *)realloc(primes_cache,new_cap * sizeof(idx_t));
    for (idx_t i = 0; i < new_cap; ++i) spf_cache[i] = 0;
    mu_cache[0] = 0;  /* unused */
    mu_cache[1] = 1;
    primes_cnt = 0;
    for (idx_t i = 2; i <= N; ++i) {
        if (!spf_cache[i]) {
            spf_cache[i] = i;
            primes_cache[primes_cnt++] = i;
            mu_cache[i] = -1;
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            idx_t p = primes_cache[j];
            idx_t ip = i * p;
            if (ip > N) break;
            spf_cache[ip] = p;
            if (p == spf_cache[i]) {
                mu_cache[ip] = 0;
                break;
            } else {
                mu_cache[ip] = -mu_cache[i];
            }
        }
    }
    mu_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/*  Cache for the sequence -----------------------------------------*/
static seq_t *a_cache      = NULL;   /* a[0..max_n]          */
static seq_t *F_cache      = NULL;   /* F[0..max_n]          */
static idx_t    cache_max  = 0;       /* largest computed n   */

/* Ensure that a[0..n] and F[0..n] are computed. */
static void ensure_cache(idx_t n)
{
    if (n <= cache_max) return;                /* already done */
    size_t new_cap = n + 1;
    a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    F_cache = (seq_t *)realloc(F_cache, new_cap * sizeof(seq_t));
    /* initialise new entries to zero (they will be filled immediately) */
    for (size_t i = cache_max + 1; i < new_cap; ++i) {
        a_cache[i] = 0;
        F_cache[i] = 0;
    }
    /* Ensure μ values up to n are available */
    init_mu(n);

    /* a[0] = 1 always */
    a_cache[0] = 1;
    /* F[0] is unused, keep it zero */
    F_cache[0] = 0;

    /* Compute new terms incrementally */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        /* 1. compute F(i) */
        seq_t Fi = (i % 2 == 0) ? 1 : (-1);        /* even -> +1, odd -> -1 */
        for (idx_t d = 1; d * d <= i; ++d) {
            if (i % d == 0) {
                idx_t k1 = d;
                if (k1 >= 2) {
                    seq_t term = (seq_t)k1 * (seq_t)k1 * F_cache[i / k1];
                    Fi -= term;
                }
                idx_t k2 = i / d;
                if (k2 != k1 && k2 >= 2) {
                    seq_t term = (seq_t)k2 * (seq_t)k2 * F_cache[i / k2];
                    Fi -= term;
                }
            }
        }
        F_cache[i] = Fi;

        /* 2. compute a(i) using the recurrence */
        seq_t sum = 0;
        for (idx_t k = 1; k <= i; ++k) {
            sum += F_cache[k] * a_cache[i - k];
        }
        a_cache[i] = sum / (seq_t)i;   /* division exact */
    }
    cache_max = n;
}

/* ------------------------------------------------------------------ */
/*  Public function ----------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    ensure_cache(n);
    return a_cache[n];
}
