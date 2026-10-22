/*
 * A369974  Dirichlet inverse of A369001, where A369001(n) = 1 if n' /
 *          gcd(n,n') is even, otherwise 0, and n' stands for the arithmetic
 *          derivative of n, A003415.
 * https://oeis.org/A369974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 2438, i16 n <= 8983, i32 n <= 8983, i64 n <= 8983,
 *            i128 n <= 8983, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = -∑_{d|n, d<n} A369001(n/d) · a(d),
 * with a(1)=1.
 * The function below memoizes both a(n) and the auxiliary
 * function A369001(n).  It works for arbitrary‐precision
 * seq_t (e.g. boost::multiprecision::cpp_int) and for the
 * fixed‐width integer types used by the build.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Utility functions provided by the framework -------------------------- */
#include "../utils/factor_u64.h"   /* void factor_u64(uint64_t n, uint64_t *pr,
                                   uint64_t *ex, size_t *cnt); */
#include "../utils/gcd.h"          /* seq_t gcd(seq_t a, seq_t b); */

/* ----------------------------------------------------------------------- */
#ifdef __cplusplus
#include <vector>
using std::vector;
#endif

/* ----------------------------------------------------------------------- */
/* Forward declaration of helper functions                              */
static unsigned char compute_f(uint64_t n);
static seq_t arithmetic_derivative(uint64_t n);

/* ----------------------------------------------------------------------- */
/* Storage for memoized values ------------------------------------------- */
#ifdef __cplusplus
/* C++ version: use std::vector for automatic destructor handling   */
static vector<seq_t> a_cache_vec(1, (seq_t)0);   /* index 0 unused */
static vector<uint8_t> f_cache_vec(1, (uint8_t)0);
#else
/* C version: use malloced arrays, but copy element‑by‑element          */
static seq_t   *a_cache    = NULL;              /* index 0 unused */
static uint8_t *f_cache    = NULL;              /* A369001 values  */
static idx_t   cache_len   = 0;                  /* number of entries allocated (>=1) */
#endif

/* ----------------------------------------------------------------------- */
/* Arithmetic derivative of n (uint64_t) --------------------------------- */
static seq_t arithmetic_derivative(uint64_t n)
{
    if (n <= 1)
        return (seq_t)0;

    uint64_t primes[64], exps[64];
    size_t   cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    seq_t res  = (seq_t)0;
    seq_t nseq = (seq_t)n;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t term = (seq_t)exps[i] * (nseq / (seq_t)primes[i]);
        res += term;
    }
    return res;
}

/* ----------------------------------------------------------------------- */
/* Sample of A369001(n): returns 1 if (n'/gcd(n,n')) is even, else 0 ---- */
static unsigned char compute_f(uint64_t n)
{
    seq_t nprime = arithmetic_derivative(n);
    seq_t g      = gcd((seq_t)n, nprime);
    if (g == 0)                   /* should not happen, but keep safe      */
        return (unsigned char)1;   /* n==1 case: gcd(1,0)=1, handled below */
    seq_t t = nprime / g;
    return ((t & (seq_t)1) == 0) ? (unsigned char)1 :
                                   (unsigned char)0;
}

/* ----------------------------------------------------------------------- */
/* Ensure caches are grown to include index n (>=1) ----------------------- */
static void ensure_cache(idx_t n)
{
#ifdef __cplusplus
    /* C++ implementation ------------------------------------------------- */
    if (n < a_cache_vec.size())
        return;                     /* already computed */

    size_t new_cap = (size_t)(n + 1);           /* indices 0..n inclusive */
    a_cache_vec.resize(new_cap, (seq_t)0);
    f_cache_vec.resize(new_cap, (uint8_t)0);

    /* Compute missing entries */
    for (size_t i = 1; i <= n; ++i)
    {
        if (i == 1) {
            a_cache_vec[i] = (seq_t)1;
            f_cache_vec[i] = compute_f((uint64_t)i);
            continue;
        }
        /* Compute f(i) first */
        f_cache_vec[i] = compute_f((uint64_t)i);

        /* Recurrence */
        seq_t sum = (seq_t)0;
        for (idx_t d = 1; d * d <= (idx_t)i; ++d)
        {
            if ((idx_t)i % d == 0)
            {
                idx_t d1 = d;
                idx_t d2 = (idx_t)i / d;

                if (d1 < (idx_t)i)
                    sum += (seq_t)f_cache_vec[d2] * a_cache_vec[d1];

                if (d2 < (idx_t)i && d2 != d1)
                    sum += (seq_t)f_cache_vec[d1] * a_cache_vec[d2];
            }
        }
        a_cache_vec[i] = -sum;
    }
#else
    /* C implementation --------------------------------------------------- */
    if (n < cache_len)  /* already computed */
        return;

    size_t new_cap = (size_t)(n + 1);          /* indices 0..n inclusive */
    seq_t  *new_a = (seq_t *)malloc(new_cap * sizeof(seq_t));
    uint8_t *new_f = (uint8_t *)malloc(new_cap * sizeof(uint8_t));
    if (!new_a || !new_f) {
        abort();                               /* allocation failed */
    }

    /* Copy old data */
    if (a_cache) {
        memcpy(new_a, a_cache, cache_len * sizeof(seq_t));
        free(a_cache);
    }
    if (f_cache) {
        memcpy(new_f, f_cache, cache_len * sizeof(uint8_t));
        free(f_cache);
    }

    a_cache = new_a;
    f_cache = new_f;

    /* Compute missing entries */
    for (size_t i = cache_len; i < new_cap; ++i)
    {
        if (i == 0) {
            a_cache[0] = (seq_t)0;
            f_cache[0] = (uint8_t)0;
            continue;
        }

        uint8_t fi = compute_f((uint64_t)i);
        f_cache[i] = fi;

        if (i == 1) {
            a_cache[1] = (seq_t)1;
            continue;
        }

        seq_t sum = (seq_t)0;
        for (idx_t d = 1; d * d <= (idx_t)i; ++d)
        {
            if ((idx_t)i % d == 0)
            {
                idx_t d1 = d;
                idx_t d2 = (idx_t)i / d;

                if (d1 < (idx_t)i)
                    sum += (seq_t)f_cache[d2] * a_cache[d1];

                if (d2 < (idx_t)i && d2 != d1)
                    sum += (seq_t)f_cache[d1] * a_cache[d2];
            }
        }
        a_cache[i] = -sum;
    }

    cache_len = new_cap;
#endif
}

/* ----------------------------------------------------------------------- */
/* Public function --------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* OEIS offset starts at 1 */
    ensure_cache(n);
#ifdef __cplusplus
    return a_cache_vec[(size_t)n];
#else
    return a_cache[(size_t)n];
#endif
}
