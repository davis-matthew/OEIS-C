/*
 * A245214  Numbers k such that A245212(k) < 0.
 * https://oeis.org/A245214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 34, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A245214 – numbers k such that
 *  A245212(k) < 0  ⇔  k * τ(k) < Σ_{d|k, d<k} d * τ(d)
 *  The function a(n) returns the n‑th such number.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- Smallest prime factor table ---------- */
static idx_t spf_limit = 1;   /* maximum index for which spf is filled */
static idx_t *spf = NULL;     /* spf[i] = smallest prime factor of i (i≥2) */

/* Extend spf table up to “limit” */
static void ensure_spf(idx_t limit)
{
    if (limit <= spf_limit) return;

    idx_t new_size = limit + 1;
    if (spf_limit == 1) {
        spf = (idx_t*)calloc(new_size, sizeof(idx_t));
    } else {
        spf = (idx_t*)realloc(spf, new_size * sizeof(idx_t));
    }

    /* initialize new entries */
    for (idx_t i = spf_limit; i <= limit; ++i) spf[i] = 0;

    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {          /* i is prime */
            spf[i] = i;
            if ((uint64_t)i * i <= limit) {
                for (idx_t j = i * i; j <= limit; j += i) {
                    if (spf[j] == 0) spf[j] = i;
                }
            }
        }
    }
    spf_limit = limit;
}

/* ---------- Storage for found sequence terms ---------- */
static size_t term_count   = 0;
static size_t term_capacity = 0;
static seq_t *terms = NULL;

/* Append a term to the list */
static void add_term(seq_t val)
{
    if (term_count == term_capacity) {
        size_t new_cap = (term_capacity == 0) ? 256 : term_capacity * 2;
        terms = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
        term_capacity = new_cap;
    }
    terms[term_count++] = val;
}

/* ---------- Recursively enumerate proper divisors ---------- */
static void dfs_divisors(int level,
                         uint64_t val,
                         uint64_t tau,
                         idx_t k,
                         const idx_t *primes,
                         const int *exps,
                         int r,
                         seq_t *sum)
{
    if (level == r) {
        if (val != k) {
            *sum += (seq_t)val * (seq_t)tau;
        }
        return;
    }
    const int e = exps[level];
    const idx_t p = primes[level];
    uint64_t p_pow = 1;
    for (int f = 0; f <= e; ++f) {
        uint64_t new_val = val * p_pow;
        uint64_t new_tau = tau * (uint64_t)(f + 1);
        dfs_divisors(level + 1, new_val, new_tau, k, primes, exps, r, sum);
        p_pow *= (uint64_t)p;
    }
}

/* ---------- Check if a given k satisfies the condition ---------- */
static bool good_k(idx_t k)
{
    /* factorisation of k using spf table */ 
    const int MAX_FACTORS = 16;   /* enough for 64‑bit integers */
    idx_t primes[MAX_FACTORS];
    int exps[MAX_FACTORS];
    int r = 0;
    idx_t tmp = k;
    while (tmp > 1) {
        idx_t p = spf[tmp];
        int e = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++e;
        }
        primes[r] = p;
        exps[r]   = e;
        ++r;
    }

    /* compute τ(k) as a seq_t value */
    seq_t tau_k = 1;
    for (int i = 0; i < r; ++i) tau_k *= (seq_t)(exps[i] + 1);

    /* compute Σ_{d|k, d<k} d * τ(d) */
    seq_t sum = 0;
    dfs_divisors(0, 1ULL, 1ULL, k, primes, exps, r, &sum);

    /* compare k * τ(k) with the sum */
    return ((seq_t)k * tau_k) < sum;
}

/* ---------- Main sequence generator ---------- */
seq_t a(idx_t n)
{
    static idx_t next_candidate = 1;      /* next k to test */

    while (term_count < n) {
        /* enlarge spf table as needed */
        if (next_candidate > spf_limit) {
            idx_t new_limit = (spf_limit == 1) ? 5000000 : spf_limit * 2;
            ensure_spf(new_limit);
        }
        if (good_k(next_candidate))
            add_term((seq_t)next_candidate);
        ++next_candidate;
    }
    return terms[n - 1];
}
