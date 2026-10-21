/*
 * A247843  Consecutive exclusionary squares: Numbers n such that n^2 does not
 *          contain digits of n and (n+1)^2 does not contain digits of n+1.
 * https://oeis.org/A247843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   digit_counts, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A247843:  Consecutive exclusionary squares
 *  a(n) = the n-th positive integer m such that
 *      the decimal digits of m^2 are disjoint from the decimal digits of m
 *      and the decimal digits of (m+1)^2 are disjoint from those of (m+1).
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#include "../utils/seq_mul.h"          /* seq_mul(seq_t a, seq_t b) */
#include "../utils/digit_counts.h"    /* digit_counts(seq_t x, unsigned char cnt[10]) */

seq_t a(idx_t n)
{
    /* Ensure n >= 1. Undefined behaviour otherwise. */
    static seq_t *cache = NULL;      /* array of found terms    */
    static size_t cache_len = 0;     /* number of terms stored  */
    static size_t cache_cap = 0;     /* allocated capacity      */
    static idx_t next_candidate = 1; /* next integer to test    */

    while (cache_len < (size_t)n)
    {
        seq_t m   = (seq_t)next_candidate;
        seq_t mp1 = m + 1;

        /* Compute squares (big integer multiplication). */
        seq_t m_sq   = seq_mul(m, m);
        seq_t mp1_sq = seq_mul(mp1, mp1);

        unsigned char cnt_m[10], cnt_m_sq[10];
        unsigned char cnt_mp1[10], cnt_mp1_sq[10];

        /* Count digits of each number. */
        digit_counts(m,     cnt_m);
        digit_counts(m_sq,  cnt_m_sq);
        digit_counts(mp1,   cnt_mp1);
        digit_counts(mp1_sq,cnt_mp1_sq);

        /* Check that digits of m^2 and m are disjoint. */
        bool ok1 = true;
        for (int d = 0; d < 10; ++d)
            if (cnt_m[d] && cnt_m_sq[d]) { ok1 = false; break; }

        /* Check that digits of (m+1)^2 and (m+1) are disjoint. */
        bool ok2 = true;
        if (ok1) {
            for (int d = 0; d < 10; ++d)
                if (cnt_mp1[d] && cnt_mp1_sq[d]) { ok2 = false; break; }
        }

        if (ok1 && ok2)
        {
            /* Store the discovered term. */
            if (cache_len >= cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0;          /* out‑of‑memory: safe‑abort */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = m;
        }

        ++next_candidate;
    }

    return cache[n - 1];
}
