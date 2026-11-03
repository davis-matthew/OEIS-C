/*
 * A320893  Numbers with an even number of prime factors (counted with
 *          multiplicity) that can be factored into squarefree semiprimes
 *          (A320911) but cannot be factored into distinct semiprimes
 *          (A320892).
 * https://oeis.org/A320893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) defined by the OEIS description for A320893. We generate
 *            the sequence by testing every integer in order, keeping a cache
 *            of the found terms
 * Range:     u8 none, u16 n <= 22, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Validated: partial, pass, 32 terms through n = 32 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------------------------------------------------------- */
/*  Smallest‑prime‑factor sieve, grown as needed.                */
/* ------------------------------------------------------------- */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* ------------------------------------------------------------- */
static bool is_graphical_simple(const uint64_t src[], size_t m)
{
    uint64_t sum = 0;
    for (size_t i = 0; i < m; ++i) sum += src[i];
    if (sum & 1ULL) return false;             /* sum must be even */

    /* copy and sort descending */
    uint64_t a[64];
    for (size_t i = 0; i < m; ++i) a[i] = src[i];
    for (size_t i = 0; i < m; ++i)
        for (size_t j = i + 1; j < m; ++j)
            if (a[j] > a[i]) {
                uint64_t t = a[i]; a[i] = a[j]; a[j] = t;
            }

    /* Erdős–Gallai inequalities */
    for (size_t k = 1; k <= m; ++k) {
        uint64_t left = 0;
        for (size_t i = 0; i < k; ++i) left += a[i];
        uint64_t right = k * (k - 1);
        for (size_t i = k; i < m; ++i)
            right += (a[i] < k) ? a[i] : k;
        if (left > right) return false;
    }
    return true;
}

/* ------------------------------------------------------------- */
static bool fails_distinct_factor(uint64_t *exps, size_t m, uint64_t bigOmega)
{
    /* Exponents array is already in exps[] */
    size_t total_masks = 1ULL << m;
    for (size_t mask = 0; mask < total_masks; ++mask) {
        uint64_t rem[64];
        bool ok = true;
        for (size_t i = 0; i < m; ++i) {
            if ((mask >> i) & 1ULL) {         /* loop at vertex i */
                if (exps[i] < 2) { ok = false; break; }
                rem[i] = exps[i] - 2;
            } else {
                rem[i] = exps[i];
            }
        }
        if (!ok) continue;
        if (is_graphical_simple(rem, m))       /* distinct semiprime possible */
            return false;
    }
    return true;                                /* cannot be distinct */
}

static uint64_t *spf = NULL;        /* smallest prime factor array     */
static uint64_t spf_limit = 0;      /* current upper bound             */

static void grow_spf(uint64_t new_limit)
{
    if (new_limit <= spf_limit) return;
    uint64_t *new_spf = (uint64_t *)malloc((new_limit + 1) * sizeof(uint64_t));
    if (!new_spf) exit(EXIT_FAILURE);
    for (uint64_t i = 0; i <= new_limit; ++i) new_spf[i] = 0;
    for (uint64_t i = 2; i <= new_limit; ++i) {
        if (!new_spf[i]) {
            new_spf[i] = i;
            if (i <= new_limit / i) {
                for (uint64_t j = i * i; j <= new_limit; j += i)
                    if (!new_spf[j]) new_spf[j] = i;
            }
        }
    }
    free(spf);
    spf = new_spf;
    spf_limit = new_limit;
}

/* ------------------------------------------------------------- */
/*  Simple‑graph realizability test (Erdős–Gallai).               */

/* ------------------------------------------------------------- */
/*  Test whether a number can be written as a product of         */
/*  distinct semiprimes.  The degrees are the prime exponents.   */

/* ------------------------------------------------------------- */
/*  Core test: does this integer n satisfy the definition?     */
/* ------------------------------------------------------------- */
static bool satisfies(uint64_t n)
{
    if (n < 2U) return false;

    uint64_t temp = n;
    uint64_t exps[64];
    size_t m = 0;
    uint64_t bigOmega = 0;
    uint64_t max_exp = 0;

    while (temp > 1U) {
        uint64_t p = spf[temp];
        uint64_t e = 0;
        while (temp % p == 0U) {
            temp /= p;
            ++e;
            ++bigOmega;
        }
        exps[m++] = e;
        if (e > max_exp) max_exp = e;
    }

    if (bigOmega & 1ULL) return false;         /* odd number of factors */

    if (max_exp > bigOmega / 2ULL) return false;/* no squarefree semiprime factorization */

    /* If it CAN be factored into distinct semiprimes, reject */
    if (!fails_distinct_factor(exps, m, bigOmega)) return false;

    return true;                                /* satisfies both conditions */
}

/* ------------------------------------------------------------- */
/*  Sequence function with memoisation.                         */
/* ------------------------------------------------------------- */
static seq_t *cache   = NULL;
static size_t  cache_cap = 0;
static size_t  cache_used = 0;
static uint64_t next_check = 2U;
static uint64_t spf_cur_limit = 1024U;          /* initial SPF limit */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0U;              /* not used for n>=1 */

    if (!cache) {
        cache_cap = 64;
        cache = mem_new(seq_t, cache_cap);
        grow_spf(spf_cur_limit);
    }

    while (cache_used < (size_t)n) {
        /* grow sieve when necessary */
        if (next_check > spf_cur_limit) {
            uint64_t new_lim = spf_cur_limit * 2U;
            grow_spf(new_lim);
            spf_cur_limit = new_lim;
        }

        if (satisfies(next_check)) {
            if (cache_used == cache_cap) {
                cache_cap <<= 1;
                cache = mem_resize(seq_t, cache, cache_cap);
            }
            cache[cache_used++] = (seq_t)next_check;
        }
        ++next_check;
    }
    return cache[n - 1];
}
