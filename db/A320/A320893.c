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
 * Helpers:   fails_distinct_factor, is_graphical_simple
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------------------------------------------------------- */
/*  Smallest‑prime‑factor sieve, grown as needed.                */
/* ------------------------------------------------------------- */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/fails_distinct_factor.h"
#include "../utils/is_graphical_simple.h"

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
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
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
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_used++] = (seq_t)next_check;
        }
        ++next_check;
    }
    return cache[n - 1];
}
