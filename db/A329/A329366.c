/*
 * A329366  Numbers whose distinct prime indices are pairwise indivisible
 *          (stable) and pairwise non-relatively prime (intersecting).
 * https://oeis.org/A329366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A329366:
   numbers n such that, for the multiset of distinct prime indices of n,
   every pair of indices is pairwise indivisible and pairwise not coprime.
   The function builds a primality table with smallest‑prime‑factor
   information and searches successive integers to collect all admissible
   values.  The sequence term a(n) is returned, with a(1)=1. */

/* ---------- standard headers ---------- */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* ---------- type aliases supplied by the build ---------- */
/* seq_t and idx_t are defined by the build system; we just use them */

/* ---------- global data for prime sieve and caching ---------- */
static uint64_t *spf        = NULL;      /* smallest prime factor, index 0..limit */
static uint64_t *primes     = NULL;      /* primes array, 0‑based */
static size_t   prime_cnt   = 0;          /* number of primes stored */
static uint64_t  sieve_lim  = 0;          /* current maximum index in spf */
static seq_t   *cache       = NULL;      /* cached sequence values */
static size_t   cache_sz    = 0;          /* number of cached terms */
static size_t   cache_cap   = 0;          /* allocated capacity for cache */
static idx_t    next_trial  = 2;          /* next integer to test */

/* ---------- helper functions ---------- */

/* Euclidean gcd for idx_t */
static idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* Extend the sieve up to at least 'limit' (inclusive) */
static void ensure_sieve(uint64_t limit)
{
    if (limit <= sieve_lim) return;

    /* Choose a new limit, at least doubling to reduce reallocs */
    uint64_t new_lim = limit;
    if (sieve_lim == 0) new_lim = limit;
    else if (new_lim <= sieve_lim * 2) new_lim = sieve_lim * 2 + 1;

    /* Allocate new arrays for spf and primes */
    uint64_t *new_spf = (uint64_t *)calloc(new_lim + 1, sizeof(uint64_t));
    if (!new_spf) exit(1);

    /* Copy old spf entries (if any) */
    if (sieve_lim > 0)
        memcpy(new_spf, spf, (sieve_lim + 1) * sizeof(uint64_t));

    /* Initialize spf for new entries */
    for (uint64_t i = sieve_lim + 1; i <= new_lim; ++i)
        new_spf[i] = i;

    /* Sieve to fill smallest prime factor for all numbers up to new_lim */
    for (uint64_t i = 2; i * i <= new_lim; ++i) {
        if (new_spf[i] == i) {          /* i is prime */
            for (uint64_t j = i * i; j <= new_lim; j += i)
                if (new_spf[j] == j)
                    new_spf[j] = i;
        }
    }

    /* Build primes array from spf data */
    size_t new_prime_cnt = 0;
    uint64_t *new_primes = (uint64_t *)malloc((new_lim / 10 + 100) * sizeof(uint64_t));
    if (!new_primes) exit(1);
    for (uint64_t i = 2; i <= new_lim; ++i)
        if (new_spf[i] == i)
            new_primes[new_prime_cnt++] = i;

    /* Replace old data */
    free(spf);
    free(primes);
    spf   = new_spf;
    primes= new_primes;
    prime_cnt = new_prime_cnt;
    sieve_lim = new_lim;
}

/* Return 1‑based index of prime 'p' in the primes array.
   Assumes p is prime and spf table has been built to cover p. */
static idx_t prime_index(uint64_t p)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        uint64_t val = primes[mid];
        if (val == p) return (idx_t)(mid + 1);  /* 1‑based index */
        if (val < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (idx_t)0;   /* should never happen for valid primes */
}

/* Append 'val' to the cached sequence, growing storage if needed */
static void append_cache(seq_t val)
{
    if (cache_sz == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_sz++] = val;
}

/* Test whether the distinct prime indices of 'x' satisfy the
   “pairwise indivisible and pairwise not coprime” condition. */
static bool admissible(uint64_t x)
{
    if (x <= 1) return true;   /* 1 has no prime indices, vacuously true */

    idx_t indices[32];
    size_t icnt = 0;
    uint64_t tmp = x;
    uint64_t last_p = 0;

    /* Factor x, collecting distinct primes and mapping them to indices */
    while (tmp > 1) {
        uint64_t p = spf[tmp];
        if (p == 0) {           /* should not happen if spf built correctly */
            return false;
        }
        if (p != last_p) {
            if (icnt >= 32) return false;   /* safety limit */
            idx_t idx = prime_index(p);
            if (idx == 0) return false;     /* prime not found in table */
            indices[icnt++] = idx;
            last_p = p;
        }
        tmp /= p;
    }

    /* Check pairwise conditions */
    for (size_t i = 0; i < icnt && true; ++i) {
        for (size_t j = i + 1; j < icnt; ++j) {
            idx_t a = indices[i], b = indices[j];
            if (a % b == 0 || b % a == 0) return false;  /* indivisible fails */
            if (gcd_idx(a, b) == 1)    return false;      /* not coprime fails */
        }
    }
    return true;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* out of range, defensive */

    /* Initialize cache on first call */
    if (cache_sz == 0) {
        append_cache((seq_t)1);      /* a(1) = 1 */
        next_trial = 2;
    }

    /* Generate terms until we have n of them */
    while (cache_sz < n) {
        /* Ensure sieve covers the candidate */
        ensure_sieve(next_trial);

        /* Test candidate */
        if (admissible(next_trial))
            append_cache((seq_t)next_trial);

        ++next_trial;   /* advance */
    }

    return cache[n - 1];
}
