/*
 * A382661  The unitary Jordan totient function applied to the exponentially
 *          odd numbers (A268335).
 * https://oeis.org/A382661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A191414(A268335(n))
 * Range:     u8 n <= 12, u16 n <= 183, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"

static seq_t *cache = NULL;          /* cache of a(1)..a(k)            */
static size_t cache_cap = 0;         /* allocated capacity              */
static size_t cache_len = 0;         /* number of terms stored          */
static uint64_t last_candidate = 0;  /* last integer checked (≥0)       */

/* Resizes the cache array to at least 'need' elements.  */
static void reserve_cache(size_t need)
{
    if (cache_len >= need)
        return;
    if (cache_cap < need) {
        size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
        if (new_cap < need)
            new_cap = need;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();                  /* out of memory – terminate */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Checks whether all prime exponents of n are odd. */
static bool exp_nonzero_even_expo(uint64_t n)
{
    if (n == 1)
        return true;
    uint64_t p[64];
    uint64_t e[64];
    size_t cnt = 0;
    factor_u64(n, p, e, &cnt);
    for (size_t i = 0; i < cnt; ++i)
        if ((e[i] & 1) == 0)       /* even exponent */
            return false;
    return true;
}

/* Computes J_2*(n) = ∏(p^(2e) - 1) over prime powers p^e | n. */
static seq_t unitary_jordan_of_odd(uint64_t n)
{
    if (n == 1)
        return (seq_t)1;
    uint64_t p[64];
    uint64_t e[64];
    size_t cnt = 0;
    factor_u64(n, p, e, &cnt);
    seq_t res = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t prime = p[i];
        uint64_t exp = e[i];
        seq_t term = pow_uint(prime, (idx_t)(2 * exp));
        term -= (seq_t)1;
        res *= term;
    }
    return res;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;     /* undefined for n == 0, but function is only used for n >= 1 */

    /* Extend cache until we have at least n terms. */
    while (cache_len < n) {
        /* Find next exponential–odd integer. */
        do {
            ++last_candidate;          /* start from 1 the first time */
        } while (!exp_nonzero_even_expo(last_candidate));

        /* Compute the value a(k) for this candidate. */
        seq_t val = unitary_jordan_of_odd(last_candidate);

        /* Store into cache. */
        reserve_cache(cache_len + 1);
        cache[cache_len++] = val;
    }

    return cache[n - 1];
}
