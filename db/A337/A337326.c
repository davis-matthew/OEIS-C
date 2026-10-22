/*
 * A337326  a(n) is the smallest number with n divisors d such that sigma(d) /
 *          tau(d) is an integer.
 * https://oeis.org/A337326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the smallest number m with n arithmetic divisors d
 *            (divisors such that σ(d)/τ(d) is integral). The implementation
 *            lazily builds tables of smallest prime factors,
 *            arithmetic-divisor counts, and the minimal m for each count up
 *            to the largest n requested
 * Range:     u8 n <= 8, u16 n <= 44, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* -------------------------------------------------------------------- */
/*              static storage and cache management                    */
/* -------------------------------------------------------------------- */

static uint32_t *spf        = NULL;  /* smallest prime factor array      */
static size_t   spf_cap    = 0;      /* capacity of spf array            */
static seq_t   *counts     = NULL;  /* arithmetic-divisor count per m   */
static size_t   counts_cap = 0;      /* capacity of counts array         */
static seq_t   *best       = NULL;  /* minimal m for each count         */
static size_t   best_cap   = 0;      /* capacity of best array           */
static idx_t    max_needed = 0;      /* largest n asked for              */

#define SENTINEL ((seq_t)-1)

/* -------------------------------------------------------------------- */
/*                utility helpers (static, internal)                    */
/* -------------------------------------------------------------------- */

static void ensure_spf(size_t limit)
{
    if (spf_cap < limit + 1) {
        uint32_t *new_spf = (uint32_t *)realloc(spf, (limit + 1) * sizeof(uint32_t));
        if (!new_spf) abort();
        spf = new_spf;
        spf_cap = limit + 1;
    }
}

static void ensure_counts(size_t limit)
{
    if (counts_cap < limit + 1) {
        seq_t *new_counts = (seq_t *)realloc(counts, (limit + 1) * sizeof(seq_t));
        if (!new_counts) abort();
        counts = new_counts;
        counts_cap = limit + 1;
    }
}

static void ensure_best(size_t size)
{
    if (best_cap < size) {
        seq_t *new_best = (seq_t *)realloc(best, size * sizeof(seq_t));
        if (!new_best) abort();
        /* initialise new entries with SENTINEL */
        for (size_t i = best_cap; i < size; ++i) {
            new_best[i] = SENTINEL;
        }
        best = new_best;
        best_cap = size;
    }
}

/* -------------------------------------------------------------------- */
/*    test whether a number n is an arithmetic divisor                */
/* -------------------------------------------------------------------- */
static bool is_arith_uint(uint64_t n)
{
    uint64_t tmp = n;
    seq_t    sigma = 1;
    uint64_t tau   = 1;

    while (tmp > 1) {
        uint64_t p = spf[tmp];
        int e = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++e;
        }

        /* compute sum_{i=0..e} p^i */
        seq_t term = 1;
        seq_t sum  = 1;
        for (int i = 0; i < e; ++i) {
            term *= p;
            sum  += term;
        }

        sigma *= sum;
        tau   *= (e + 1);
    }
    return (sigma % (seq_t)tau) == 0;
}

/* -------------------------------------------------------------------- */
/*        recompute tables up to a given limit (from scratch)           */
/* -------------------------------------------------------------------- */
static void recompute(size_t limit)
{
    /* 1. Build smallest prime factor array using simple sieve */
    ensure_spf(limit);
    for (size_t i = 0; i <= limit; ++i) spf[i] = 0;
    if (limit >= 1) spf[1] = 1;
    for (size_t i = 2; i <= limit; ++i) spf[i] = (uint32_t)i;

    for (size_t i = 2; i * i <= limit; ++i) {
        if (spf[i] == i) {
            for (size_t j = i * i; j <= limit; j += i) {
                if (spf[j] == j) spf[j] = (uint32_t)i;
            }
        }
    }

    /* 2. Re‑initialise counts */
    ensure_counts(limit);
    for (size_t i = 1; i <= limit; ++i) counts[i] = 0;

    /* 3. Count arithmetic divisors for all numbers up to limit */
    for (size_t d = 1; d <= limit; ++d) {
        if (is_arith_uint(d)) {
            for (size_t j = d; j <= limit; j += d) {
                ++counts[j];
            }
        }
    }

    /* 4. Update best table for all counts up to the largest needed n */
    for (size_t m = 1; m <= limit; ++m) {
        idx_t c = (idx_t)counts[m];
        if (c <= max_needed) {
            if (best[(size_t)c] == SENTINEL || (seq_t)m < best[(size_t)c]) {
                best[(size_t)c] = (seq_t)m;
            }
        }
    }
}

/* -------------------------------------------------------------------- */
/*                                 Main sequence                         */
/* -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for n=0, but return 0 */

    /* make sure the 'best' array is large enough */
    ensure_best((size_t)n + 1);

    /* if we already know the answer, return it */
    if (best[(size_t)n] != SENTINEL) return best[(size_t)n];

    /* keep track of the largest n requested so far */
    if ((idx_t)max_needed < n) max_needed = n;

    /* grow the limit until we have found a(n) */
    size_t limit = 1;
    while (best[(size_t)n] == SENTINEL) {
        if (limit < 2) limit = 2;
        else          limit <<= 1;            /* double the limit */
        recompute(limit);
    }
    return best[(size_t)n];
}
