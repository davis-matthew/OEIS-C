/*
 * A241928  a(n) = smallest k such that lambda(n+k) = lambda(k).
 * https://oeis.org/A241928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k such that λ(n + k) = λ(k) (λ = Carmichael
 *            lambda)
 * Range:     u8 n <= 35, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 89 terms through n = 9414 (2026-11-03)
 * Helpers:   build_primes_upto, gcd, ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* utility headers (assume they exist in the build environment) */
#include <stdint.h>
#include "../utils/build_primes_upto.h"
#include "../utils/gcd.h"
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Grow a dynamic array when needed.  The pointer is passed as OEISC_VPP(.) */
#ifdef __cplusplus
template <class T>
static void ensure_capacity(size_t needed, size_t *cap, T **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    T *tmp = mem_resize(T, *ptr, (new_cap * elem_size) / sizeof(T));
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#else
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#endif

/* ------------------------------------------------------------------------- */
/*  Cached Carmichael lambda values                                         */
/* ------------------------------------------------------------------------- */
static seq_t *lambda_cache = NULL;  /* array of λ values, indexed by m */
static size_t lambda_cache_cap = 0; /* current allocated size */

/* Ensure λ_cache has space for indices up to n (inclusive).  New slots
   are cleared to 0 so a non‑zero value always means "cached". */
static void ensure_lambda_capacity(idx_t n)
{
    if (n + 1 <= lambda_cache_cap)
        return;

    size_t old_cap = lambda_cache_cap;
    size_t needed = (size_t)(n + 1);
    ensure_capacity(needed, &lambda_cache_cap, OEISC_VPP(&lambda_cache), sizeof(seq_t));

    /* initialize the newly added elements to 0 */
    for (size_t i = old_cap; i < lambda_cache_cap; ++i)
        lambda_cache[i] = (seq_t)0;
}

/* ------------------------------------------------------------------------- */
/*  Prime number utilities                                                  */
/* ------------------------------------------------------------------------- */
static uint64_t *primes = NULL;   /* array of primes */
static size_t primes_cnt = 0;     /* number of primes currently stored */
static idx_t primes_limit = 0;    /* largest prime currently precomputed */

/* Extend the prime list so that all primes <= limit are available. */
static void ensure_primes(idx_t limit)
{
    if (limit <= primes_limit)
        return;

    free(primes);
    build_primes_upto(limit, &primes, &primes_cnt);
    primes_limit = limit;
}

/* ------------------------------------------------------------------------- */
/*  Carmichael lambda of a single integer m                                 */
/* ------------------------------------------------------------------------- */

/* Compute λ(m) for m ≥ 1.  The result fits in seq_t for all m that arise. */
static seq_t compute_lambda(idx_t m)
{
    if (m == 1)
        return (seq_t)1;

    seq_t lam = (seq_t)1;
    idx_t temp = m;

    /* Determine an upper bound for trial division */
    idx_t sqrt_lim = (idx_t)floor_sqrt((seq_t)temp);
    if (sqrt_lim < 2)
        sqrt_lim = 2;

    ensure_primes(sqrt_lim);

    for (size_t i = 0; i < primes_cnt && primes[i] <= sqrt_lim; ++i) {
        uint64_t p = primes[i];
        if (temp % p != 0)
            continue;

        int exp = 0;
        while (temp % p == 0) {
            temp /= p;
            exp++;
        }

        seq_t lam_pe;
        if (p == 2) {
            /* λ(2) = 1, λ(4) = 2, λ(2^e) = 2^(e-2) for e ≥ 3 */
            if (exp == 1)
                lam_pe = (seq_t)1;
            else if (exp == 2)
                lam_pe = (seq_t)2;
            else {
                lam_pe = (seq_t)1;
                for (int j = 0; j < exp - 2; ++j)
                    lam_pe *= (seq_t)2;
            }
        } else {
            /* For odd primes p^e: λ(p^e) = φ(p^e) = p^(e-1)·(p-1) */
            seq_t pow = (seq_t)1;
            for (int j = 0; j < exp - 1; ++j)
                pow *= (seq_t)p;
            lam_pe = pow * (seq_t)(p - 1);
        }

        seq_t g = gcd(lam, lam_pe);
        lam = (lam / g) * lam_pe;  /* lcm(lam, lam_pe) */
    }

    /* Any remaining factor is a prime > sqrt_lim. */
    if (temp > 1) {
        seq_t lam_pe = (seq_t)temp - (seq_t)1;  /* φ(temp) for prime */
        seq_t g = gcd(lam, lam_pe);
        lam = (lam / g) * lam_pe;
    }

    return lam;
}

/* Retrieve λ(m) from cache; compute if necessary. */
static seq_t lambda_of(idx_t m)
{
    if (m == 1)
        return (seq_t)1;

    ensure_lambda_capacity(m);
    if (lambda_cache[m] != 0)
        return lambda_cache[m];

    lambda_cache[m] = compute_lambda(m);
    return lambda_cache[m];
}

/* ------------------------------------------------------------------------- */
/*  Main sequence function                                                  */
/* ------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Prepare the cache for λ(1).  λ(1) = 1. */
    ensure_lambda_capacity(1);
    lambda_cache[1] = (seq_t)1;

    /* Search for the smallest k ≥ 1 such that λ(n + k) = λ(k). */
    for (idx_t k = 1; ; ++k) {
        seq_t lam_nk = lambda_of(n + k);
        seq_t lam_k   = lambda_of(k);
        if (lam_nk == lam_k)
            return (seq_t)k;
    }
}
