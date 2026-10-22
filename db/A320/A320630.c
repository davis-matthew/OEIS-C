/*
 * A320630  Products of primes of nonprime squarefree index.
 * https://oeis.org/A320630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th number (excluding 1) in the strictly increasing
 *            sequence of all positive integers whose prime factors are
 *            exactly the primes whose 1‑based index among all primes is a
 *            nonprime square‑free integer
 * Range:     u8 none, u16 none, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   build_primes_upto, build_spf, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The algorithm is the standard “ugly‑numbers” multi‑pointer method.
    It keeps an array “next[i] = a[pos[i]] * p[i]”, where p[i] is an allowed
    prime and pos[i] is the index in the sequence a (starting with a[0] = 1).
    The next term a(k) is the minimum over all next[i].  After choosing it,
    all indices whose candidate equals this minimum are advanced.  The
    algorithm runs in O(n * k) of very small operations; for n ≤ 10 000
    the set of allowed primes k is only a few hundred.  It uses only
    integer arithmetic, no floating point or large algorithmic constants. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/build_spf.h"
#include "../utils/is_prime.h"

/*------------------------------------*/
/* Build the set of allowed primes    */
/*------------------------------------*/
#define PRIME_NUMERIC_LIMIT 2000000ULL   /* enough to generate a huge set     */

static uint64_t *g_all_primes    = NULL;  /* primes up to numeric limit      */
static size_t   g_all_prime_cnt  = 0;     /* number of primes obtained       */
static idx_t   *g_spf            = NULL;  /* smallest prime factor array     */
static seq_t   *g_allowed_primes = NULL;  /* primes whose indices are
                                             nonprime square‑free           */
static size_t   g_allowed_cnt   = 0;       /* number of allowed primes        */
static bool     g_initialized   = false;

/* Helper: factor idx using SPF to test squarefree */
static bool is_nonprime_squarefree(idx_t idx)
{
    if (idx == 1)          /* 1 is considered nonprime squarefree */
        return true;
    if (idx == 0)          /* never reached but safe */
        return false;

    if (is_prime(idx))     /* skip prime indices */
        return false;

    /* factor using spf: multiplicities > 1 => not squarefree */
    idx_t m = idx;
    while (m > 1) {
        idx_t p = g_spf[m];
        m /= p;
        if (m % p == 0)    /* p^2 divides idx */
            return false;
    }
    return true;
}

/* Initialize static data structures once. */
static void init_static(void)
{
    if (g_initialized)
        return;

    /* 1. generate all primes up to numeric limit */
    build_primes_upto(PRIME_NUMERIC_LIMIT, &g_all_primes, &g_all_prime_cnt);

    /* 2. build SPF for indices 1..prime_cnt */
    idx_t prime_limit = (idx_t)g_all_prime_cnt;
    build_spf(prime_limit, &g_spf);

    /* 3. collect allowed primes */
    g_allowed_primes = (seq_t *)malloc(sizeof(seq_t) * g_all_prime_cnt);
    if (!g_allowed_primes) abort();  /* allocation failure */
    g_allowed_cnt = 0;
    for (size_t i = 0; i < g_all_prime_cnt; ++i) {
        idx_t idx = (idx_t)(i + 1);             /* 1‑based index */
        if (is_nonprime_squarefree(idx)) {
            g_allowed_primes[g_allowed_cnt++] =
                (seq_t)g_all_primes[i]; /* cast to seq_t, fits if seq_t ≥ 64‑bit */
        }
    }

    g_initialized = true;
}

/*------------------------------------*/
/* Main function                       */
/*------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                     /* undefined below offset */

    init_static();

    /* Allocate sequence array: a[0] = 1 (virtual), then a[1..n] are terms */
    seq_t *seq = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    if (!seq)
        return (seq_t)0;                     /* allocation failed */

    seq[0] = (seq_t)1;                       /* virtual starting point */

    /* Pointers into seq for each allowed prime */
    idx_t *ptr = (idx_t *)calloc(g_allowed_cnt, sizeof(idx_t));
    if (!ptr) {
        free(seq);
        return (seq_t)0;
    }

    /* Current candidates: next[i] = seq[ptr[i]] * msg_allowed_primes[i] */
    seq_t *cand = (seq_t *)calloc(g_allowed_cnt, sizeof(seq_t));
    if (!cand) {
        free(seq);
        free(ptr);
        return (seq_t)0;
    }

    for (size_t i = 0; i < g_allowed_cnt; ++i)
        cand[i] = seq[ptr[i]] * g_allowed_primes[i];   /* ptr[i]==0 initially */

    /* Generate terms a(1)..a(n) */
    for (idx_t k = 1; k <= n; ++k) {
        /* Find minimum candidate */
        seq_t min_val = cand[0];
        size_t min_idx = 0;
        for (size_t i = 1; i < g_allowed_cnt; ++i) {
            if (cand[i] < min_val) {
                min_val = cand[i];
                min_idx = i;
            }
        }

        seq[k] = min_val;                      /* store term */

        /* Advance all pointers whose candidate equals min_val */
        for (size_t i = 0; i < g_allowed_cnt; ++i) {
            if (cand[i] == min_val) {
                ++ptr[i];
                /* seq[ptr[i]] now exists because we generate in order */
                cand[i] = seq[ptr[i]] * g_allowed_primes[i];
            }
        }
    }

    seq_t result = seq[n];
    free(seq);
    free(ptr);
    free(cand);
    return result;
}
