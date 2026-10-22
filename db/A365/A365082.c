/*
 * A365082  Prime powers (A246655) q such that -2 is a nonzero square in the
 *          finite field F_q.
 * https://oeis.org/A365082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 514, u32 n <= 4203, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = n-th element of the increasing sequence of prime powers q such that
 *   (-2)^((q-1)/2) = 1 in the field F_q.
 * The sequence consists of all odd prime powers p^k where
 *   k >= 1 if p ≡ 1 or 3 (mod 8), or
 *   k is even, k >= 2 if p ≡ 5 or 7 (mod 8).
 *
 * The implementation generates prime powers up to a numeric limit
 * and keeps an ascending cached list.  The cache is grown until
 * at least n terms have been produced.
 *
 * External helper: build_primes(limit, &primes, &cnt)  (see utils/build_primes.h)
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes.h"   /* provides: void build_primes(idx_t, idx_t **, size_t *) */

static int cmp_seq_t(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* Cached sequence data */
static seq_t   *terms      = NULL;
static size_t  terms_len   = 0;
static size_t  terms_cap   = 0;
static seq_t   cur_limit   = 0;    /* numeric limit up to which terms are generated */

/* Generate all prime powers ≤ numeric_limit and store them in ascending order. */
static void generate_terms_up_to(seq_t numeric_limit)
{
    /* build_primes requires a 64‑bit limit; cast safely for typical usage. */
    idx_t limit_idx = (idx_t)numeric_limit;
    idx_t *prime_tbl;
    size_t prime_cnt;

    build_primes(limit_idx, &prime_tbl, &prime_cnt);

    /* Rough estimate of capacity; will grow if needed. */
    size_t capacity = prime_cnt * 4;
    if (capacity < 16) capacity = 16;
    seq_t *tmp = (seq_t *)malloc(sizeof(seq_t) * capacity);
    size_t tmp_len = 0;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = (uint64_t)prime_tbl[i];
        if (p < 3) continue;   /* skip prime 2 */

        bool p_is_1or3 = ((p & 7ULL) == 1ULL || (p & 7ULL) == 3ULL);

        seq_t val, mul;
        if (p_is_1or3) {
            val = (seq_t)p;
            mul = (seq_t)p;                 /* multiply by p for next power */
        } else {
            val = mul = (seq_t)p * (seq_t)p; /* start at p², multiply by p² each step */
        }

        /* Store all powers ≤ numeric_limit. */
        while (val <= numeric_limit) {
            /* Grow temporary array if necessary. */
            if (tmp_len == capacity) {
                capacity <<= 1;
                tmp = (seq_t *)realloc(tmp, sizeof(seq_t) * capacity);
            }
            tmp[tmp_len++] = val;

            /* Check for overflow before next multiplication. */
            if (mul == 0) break;                     /* safety for degenerate types */
            if (val > numeric_limit / mul) break;    /* further multiplication would overflow */

            val *= mul;
        }
    }

    free(prime_tbl);

    /* Sort and store the generated values. */
    qsort(tmp, tmp_len, sizeof(seq_t), cmp_seq_t);
    free(terms);
    terms      = tmp;
    terms_len  = tmp_len;
    terms_cap  = capacity;
    cur_limit  = numeric_limit;
}

/* Public sequence accessor.  Calls are independent of order. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* out of bounds */

    /* Ensure at least n terms are available. */
    while (terms_len < (size_t)n) {
        seq_t next_limit = (cur_limit == 0) ? (seq_t)10 : cur_limit * 2;
        generate_terms_up_to(next_limit);
    }
    return terms[n - 1];
}
