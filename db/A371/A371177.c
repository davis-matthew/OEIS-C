/*
 * A371177  Positive integers whose prime indices include all distinct
 *          divisors of all prime indices.
 * https://oeis.org/A371177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 5514, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes OEIS sequence A371177
 *  A(n)= smallest positive integer such that
 *  #distinct prime factors(m) = #distinct divisors of all prime indices of m
 *
 *  The implementation searches integers in increasing order,
 *  caching found terms so that subsequent calls with higher indices
 *  are fast.  Prime indices are obtained from a pre‑computed list of primes.
 */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/factor_u64.h"

static uint64_t *prime_list     = NULL;   /* prime list (1‑based index)    */
static size_t   prime_cnt       = 0;      /* number of primes              */

static bool     *div_mark       = NULL;   /* marks divisors of prime indices */
static size_t   *touched        = NULL;   /* indices marked during a test   */
static size_t    touched_cap    = 0;      /* capacity of touched array      */

static seq_t   *terms          = NULL;   /* cached sequence values         */
static size_t   terms_cnt      = 0;      /* number of cached terms         */
static size_t   terms_cap      = 0;      /* allocated capacity of terms    */

static idx_t    next_candidate = 1;       /* next integer to test           */

/* ------------------------------------------------------------------ */
/* Initialise global data once.                                        */
static void init_global(void)
{
    if (prime_list != NULL) return;          /* already initialised */

    /* -------------------------------------------------------------
     * Build a prime list large enough to cover the first ~20000
     * terms of the sequence.  The exact bound is not critical as
     * the sequence grows slowly; 2 × 10⁶ gives ~148 933 primes.
     * -------------------------------------------------------------
     */
    const idx_t PRIME_LIMIT = 2000000ULL;      /* adjust if needed */
    build_primes_upto(PRIME_LIMIT, &prime_list, &prime_cnt);

    /* Allocate divisor marking array (indices are 1‑based). */
    div_mark = (bool *)calloc(prime_cnt + 1, sizeof(bool));

    /* Approximate size for temporarily tracked divisors. */
    touched_cap = 200000;                     /* generous initial capacity */
    touched     = (size_t *)malloc(touched_cap * sizeof(size_t));

    /* Allocate cache for sequence terms. */
    terms_cap = 10000;
    terms     = (seq_t *)malloc(terms_cap * sizeof(seq_t));
    terms_cnt = 0;
}

/* ------------------------------------------------------------------ */
/* Binary search for 1‑based index of a prime p in prime_list. */
static size_t prime_index(uint64_t p)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (prime_list[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo + 1;     /* 1‑based index */
}

/* ------------------------------------------------------------------ */
/* Recursively enumerate all divisors of n and mark them. */
static void enumerate_divisors(const uint64_t *pf, const uint64_t *exp,
                               size_t npf, uint64_t current,
                               size_t *set_count, size_t *tcount)
{
    if (npf == 0) {
        if (!div_mark[current]) {
            div_mark[current] = true;
            if (*tcount >= touched_cap) {
                touched_cap *= 2;
                touched = (size_t *)realloc(touched, touched_cap * sizeof(size_t));
            }
            touched[(*tcount)++] = current;
            ++(*set_count);
        }
        return;
    }
    uint64_t p = pf[0];
    uint64_t mult = 1;
    for (unsigned e = 0; e <= exp[0]; ++e) {
        enumerate_divisors(pf + 1, exp + 1, npf - 1, current * mult,
                           set_count, tcount);
        mult *= p;
    }
}

/* ------------------------------------------------------------------ */
/* Test whether an integer m satisfies the defining property. */
static bool test_candidate(idx_t m)
{
    /* Factor m to get its distinct prime factors. */
    uint64_t primes_f[32];
    uint64_t exps_f[32];
    size_t cnt_f = 0;
    factor_u64((uint64_t)m, primes_f, exps_f, &cnt_f);

    size_t omega = cnt_f;                     /* #distinct prime factors */

    size_t set_count = 0;                     /* #distinct divisors of indices */
    size_t tcount    = 0;                     /* #entries in touched array */

    for (size_t i = 0; i < cnt_f; ++i) {
        uint64_t p = primes_f[i];
        size_t idx = prime_index(p);          /* index of p (1‑based) */

        /* Factor idx to enumerate its divisors. */
        uint64_t primes_idx[32];
        uint64_t exps_idx[32];
        size_t cnt_idx = 0;
        factor_u64(idx, primes_idx, exps_idx, &cnt_idx);

        enumerate_divisors(primes_idx, exps_idx, cnt_idx,
                           1ULL, &set_count, &tcount);
    }

    /* Clear marks for next candidate. */
    for (size_t i = 0; i < tcount; ++i)
        div_mark[touched[i]] = false;

    return set_count == omega;
}

/* ------------------------------------------------------------------ */
/* Public function.  Returns the nth term (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for n=0 */

    init_global();

    /* Ensure the terms array can hold at least n entries. */
    if (n > terms_cap) {
        terms_cap = n * 2;
        terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
    }

    /* Find terms up to index n. */
    while (terms_cnt < n) {
        if (test_candidate(next_candidate))
            terms[terms_cnt++] = (seq_t)next_candidate;
        ++next_candidate;
    }

    return terms[n - 1];
}
