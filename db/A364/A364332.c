/*
 * A364332  a(n) = f(prime(n)), where f(2) = 0 and for an odd prime p, f(p) =
 *          max{f(q)+1: q ranges over all prime factors of p-1}.
 * https://oeis.org/A364332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = f(prime(n)) where f(2) = 0 and for odd prime p, f(p) =
 *            max{f(q)+1 : q is a prime factor of (p-1)}
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, build_spf, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* Utility headers provided by the build system */
#include "../utils/factor_u64.h"
#include "../utils/build_primes_upto.h"
#include "../utils/build_spf.h"

/* ------------------------------------------------------------------------ */
/* Global tables and limits                                               */
/* ------------------------------------------------------------------------ */
static uint64_t *prime_arr = NULL;      /* 0‑based array of primes          */
static size_t    prime_count = 0;       /* number of primes stored          */

static idx_t   *spf = NULL;             /* smallest prime factor array      */
static size_t   spf_limit = 0;          /* maximum index spf covers         */

static seq_t  *f_cache = NULL;          /* memoised f(p) values             */
static bool   *f_valid = NULL;          /* validity flags for f_cache       */
static size_t   f_limit = 0;             /* maximum index f_cache covers     */

/* ------------------------------------------------------------------------ */
/* Helper: ensure we have at least 'need' primes                          */
/* ------------------------------------------------------------------------ */
static void ensure_primes(size_t need)
{
    static idx_t prime_lmt = 2;
    if (prime_count >= need) return;

    while (prime_count < need) {
        prime_lmt *= 2;
        if (prime_arr) free(prime_arr);
        build_primes_upto(prime_lmt, &prime_arr, &prime_count);
    }
}

/* ------------------------------------------------------------------------ */
/* Helper: ensure spf array covers indices up to 'limit'                     */
/* ------------------------------------------------------------------------ */
static void ensure_spf(size_t limit)
{
    if (spf_limit >= limit) return;

    if (spf) free(spf);
    build_spf((idx_t)(limit + 1), &spf);   /* array size: limit+1 */
    spf_limit = limit;
}

/* ------------------------------------------------------------------------ */
/* Helper: ensure cache covers indices up to 'n'                            */
/* ------------------------------------------------------------------------ */
static void ensure_f_cache(size_t n)
{
    if (f_limit >= n) return;
    size_t new_limit = f_limit ? f_limit : 1;
    while (new_limit <= n) new_limit <<= 1;   /* grow by factor 2 */

    seq_t *new_cache = (seq_t *)realloc(f_cache,
                                      (new_limit + 1) * sizeof(seq_t));
    bool  *new_valid = (bool *)realloc(f_valid,
                                      (new_limit + 1) * sizeof(bool));

    /* initialise newly allocated part */
    for (size_t i = f_limit + 1; i <= new_limit; ++i) new_valid[i] = false;

    f_cache = new_cache;
    f_valid = new_valid;
    f_limit = new_limit;
}

/* ------------------------------------------------------------------------ */
/* Recursive computation of f(p)                                            */
/* ------------------------------------------------------------------------ */
static seq_t f_prime(uint64_t p)
{
    if (p == 2) return 0;

    ensure_f_cache((size_t)p);          /* make sure we can index by p */
    if (f_valid[p]) return f_cache[p];

    /* factor p-1 into distinct prime factors */
    uint64_t fac[64], exp[64];
    size_t cnt = 0;
    factor_u64(p - 1, fac, exp, &cnt);

    seq_t max_f = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t q = fac[i];
        seq_t v = f_prime(q);
        if (v > max_f) max_f = v;
    }

    seq_t result = max_f + 1;
    f_cache[p] = result;
    f_valid[p] = true;
    return result;
}

/* ------------------------------------------------------------------------ */
/* Public sequence function                                                  */
/* ------------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, but safe */

    /* ensure we have the n-th prime */
    ensure_primes((size_t)n);
    uint64_t p = prime_arr[n - 1];   /* prime array is 0‑based */

    /* ensure spf up to p-1 for upcoming factorizations */
    if (p > 1) ensure_spf((size_t)(p - 1));

    return f_prime(p);
}
