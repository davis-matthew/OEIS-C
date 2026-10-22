/*
 * A356907  Expansion of 1 / (1 + Sum_{k>=1} lambda(k)*x^k), where lambda() is
 *          the Liouville function (A008836).
 * https://oeis.org/A356907
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 13, i16 n <= 27, i32 n <= 40, i64 n <= 40, i128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A356907
 *  a(0) = 1
 *  a(n) = -SUM_{k=1..n} λ(k) · a(n-k)   (n ≥ 1)
 *  λ(k) – Liouville function (±1)
 *  The implementation uses a linear sieve to compute λ(k) and
 *  memoises a(n) for all n already requested.  All allocations are
 *  explicitly cast to avoid C++ implicit-conversion errors.
 */

#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

/* ---------- cache data ------------------------------------------- */
static seq_t  *a_cache   = NULL;   /* a[0 .. a_size-1] */
static size_t a_cap     = 0;       /* allocated capacity of a_cache */
static size_t a_size    = 0;       /* number of stored values (zero means no cache) */

/* ---------- helper: grow a_cache up to index n ------------------- */
static void compute_up_to(idx_t n)
{
    if ((size_t)n + 1 > a_cap) {
        seq_t *tmp = (seq_t*)realloc(a_cache, ((size_t)n + 1) * sizeof(seq_t));
        if (!tmp) abort();
        a_cache = tmp;
        a_cap   = (size_t)n + 1;
    }

    /* Allocate prime‑factor tables for 1 … n */
    size_t sz   = (size_t)n + 1;
    idx_t   *spf = (idx_t*)calloc(sz, sizeof(idx_t));      /* smallest prime factor */
    int8_t  *lam = (int8_t*)calloc(sz, sizeof(int8_t));    /* Liouville values */
    idx_t   *pr  = (idx_t*)malloc(sz * sizeof(idx_t));    /* primes list */
    if (!spf || !lam || !pr) abort();
    size_t pc = 0;                                        /* primes count */

    lam[1] = 1;                                           /* λ(1)=1 */

    /* Linear sieve: fill spf and primes */
    for (idx_t i = 2; i <= n; ++i) {
        if (spf[(size_t)i] == 0) {                       /* i is prime */
            spf[(size_t)i] = i;
            pr[pc++] = i;
        }
        for (size_t j = 0; j < pc &&
                           pr[j] <= spf[(size_t)i] &&
                           i * pr[j] <= n; ++j) {
            idx_t v = i * pr[j];
            if (spf[(size_t)v] == 0)
                spf[(size_t)v] = pr[j];
        }
    }

    /* Compute λ(k) for k ≥ 2 */
    for (idx_t i = 2; i <= n; ++i)
        lam[(size_t)i] = -lam[(size_t)(i / spf[(size_t)i])];

    /* Extend the a_cache using the recurrence */
    for (size_t i = a_size; i <= (size_t)n; ++i) {
        seq_t sum = 0;
        /* a(0)=1 is already stored in a_cache[0] */
        for (size_t k = 1; k <= i; ++k) {
            if (lam[k] == 1)        /* λ(k)= 1 : subtract a(i-k) */
                sum -= a_cache[i - k];
            else                    /* λ(k)=-1 : add a(i-k)   */
                sum += a_cache[i - k];
        }
        a_cache[i] = sum;
    }
    a_size = (size_t)n + 1;

    /* Clean up temporary tables */
    free(spf);
    free(lam);
    free(pr);
}

/* ---------- public function ------------------------------------- */
seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0) return (seq_t)1;

    /* initialise cache on first call */
    if (!a_cache) {
        a_cache = (seq_t*)malloc(sizeof(seq_t));
        if (!a_cache) abort();
        a_cap   = 1;
        a_cache[0] = (seq_t)1;
        a_size   = 1;
    }

    /* extend cache if needed */
    if ((size_t)n + 1 > a_size)
        compute_up_to(n);

    return a_cache[(size_t)n];
}
