/*
 * A279815  Numbers n such that the average of the squares of the numbers less
 *          than n that do not divide n is an integer.
 * https://oeis.org/A279815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 72, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A279815: numbers n such that the average of the squares of
   the numbers less than n that do not divide n is an integer.
   Formula: a(n) = the n‑th such integer, indexed from a(1)=3. */

/* This implementation searches for the terms on demand and caches the
 * results.  It uses the helper function factor_u64 to factor each tested
 * n.  The code relies on the utilities provided by the build system. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

/* Cache for the sequence. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t candidate = 3;   /* next integer to test */

/* Test whether a candidate m satisfies the definition. */
static bool satisfies(uint64_t m)
{
    /* For m = 1 or 2 there are no numbers less than m, so return false. */
    if (m <= 2)
        return false;

    /* Factorise m. */
    uint64_t pr[64], ex[64];
    size_t pf_cnt = 0;
    factor_u64(m, pr, ex, &pf_cnt);

    /* Compute τ(m) and sum of squares of all divisors of m. */
    seq_t tau = 1;
    seq_t sum_div_sq = 1;

    for (size_t i = 0; i < pf_cnt; ++i) {
        uint64_t p  = pr[i];
        unsigned  e = (unsigned)ex[i];

        tau *= (seq_t)(e + 1);

        /* sum_{k=0..e} p^{2k} */
        seq_t sum_factor = 1;
        seq_t pk          = 1;
        seq_t p_sq        = (seq_t)p * (seq_t)p;
        for (unsigned k = 1; k <= e; ++k) {
            pk = pk * p_sq;
            sum_factor += pk;
        }
        sum_div_sq *= sum_factor;
    }

    seq_t m_seq = (seq_t)m;
    seq_t m_sq  = m_seq * m_seq;

    /* Sum of squares of proper divisors (excluding m itself). */
    seq_t sum_div_sq_proper = sum_div_sq - m_sq;

    /* Total sum of squares 1..m-1. */
    seq_t total_sq = (seq_t)(m - 1) * m_seq * (seq_t)(2 * m - 1) / 6;

    /* Count of non‑divisors of m among 1..m-1. */
    seq_t count_nondiv = m_seq - tau;
    if (count_nondiv == 0)
        return false;

    /* Sum of squares of non‑divisors. */
    seq_t sum_nondiv = total_sq - sum_div_sq_proper;

    /* The average is integer iff sum_nondiv % count_nondiv == 0. */
    return (sum_nondiv % count_nondiv) == 0;
}

/* Ensure that the cache contains at least n terms. */
static void ensure_cache(idx_t n)
{
    while (cache_len < (size_t)n) {
        if (satisfies(candidate)) {
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_cache) abort();
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)candidate;
        }
        ++candidate;
    }
}

/* Main function required by the build system. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n<1, but not called  */
    ensure_cache(n);
    return cache[(size_t)(n - 1)];
}
