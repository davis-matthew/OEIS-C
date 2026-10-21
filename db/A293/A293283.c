/*
 * A293283  Numbers n such that n^2 = a^2 + b^5 for positive integers a b and
 *          n.
 * https://oeis.org/A293283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   factor_b_power5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n): the n-th smallest positive integer n for which ∃a,b>0 with
   n² = a² + b⁵.  The sequence is obtained by enumerating all pairs
   (d,e) of divisors of b⁵ with e>d, both of the same parity, and
   setting n = (d+e)/2.  The resulting n‑values are sorted and
   deduplicated.  The function works for any seq_t/idx_t types
   provided by the build environment. */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_b_power5.h"

/* ------------------------------------------------------------ */
/* Portable comparison routine for seq_t values                  */
static int cmp_seq(const void *p1, const void *p2)
{
    seq_t v1 = *(const seq_t *)p1;
    seq_t v2 = *(const seq_t *)p2;
    if (v1 < v2) return -1;
    if (v1 > v2) return  1;
    return 0;
}

/* ------------------------------------------------------------ */
/* Global cache for sequence values                             */
static seq_t *seq_cache = NULL;
static size_t   seq_len  = 0;
static size_t   seq_cap  = 0;
static uint32_t current_bmax = 0;

/* Ensure cache has at least one allocation of the requested size */
static void grow_cache(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    seq_cache = tmp;
    seq_cap   = new_cap;
}

/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */
/* Recursive divisor generation that directly pushes valid n
   values into the candidate array 'cand'.                      */
static void gen_divisors_rec(uint64_t cur,
                             int idx,
                             const uint32_t *primes,
                             const uint8_t *exps,
                             int num_factors,
                             uint64_t bf,
                             seq_t *cand,
                             size_t *cand_len)
{
    if (idx == num_factors)
    {
        uint64_t d = cur;
        uint64_t e = bf / d;
        if (d >= e)                         /* ensure d < e */
            return;
        if ((d ^ e) & 1ULL)                 /* parity must match */
            return;
        seq_t n_val = ((seq_t)d + (seq_t)e) >> 1;
        cand[(*cand_len)++] = n_val;
        return;
    }

    uint32_t p = primes[idx];
    uint8_t  exp = exps[idx];
    unsigned __int128 prod = 1;
    for (uint8_t e = 0; e <= exp; ++e)
    {
        uint64_t new_cur = cur * (uint64_t)prod;
        if (new_cur > bf)                    /* safety, though not needed */
            break;
        gen_divisors_rec(new_cur, idx + 1,
                         primes, exps, num_factors,
                         bf, cand, cand_len);
        prod *= p;
    }
}

/* ------------------------------------------------------------ */
/* Build the list of n values for all b ≤ limit and store the
   sorted, deduplicated result in the global cache.              */
static void compute_sequence(uint32_t limit)
{
    /* Allocate a generous candidate buffer.  Rough estimate:
       each b produces ≤ 512 candidates; guess 512 per b. */
    size_t cand_cap = (size_t)limit * 512;
    seq_t *cand = (seq_t *)malloc(cand_cap * sizeof(seq_t));
    if (!cand) exit(EXIT_FAILURE);
    size_t cand_len = 0;

    uint32_t primes[32];
    uint8_t  exps[32];
    int num_factors;

    for (uint32_t b = 1; b <= limit; ++b)
    {
        /* compute b⁵ in 128‑bit and check if it fits in 64‑bit */
        unsigned __int128 bf128 = 1;
        for (int i = 0; i < 5; ++i) bf128 *= (unsigned __int128)b;
        if ((bf128 >> 64) != 0) continue;          /* overflow, skip */
        uint64_t bf = (uint64_t)bf128;

        factor_b_power5(b, primes, exps, &num_factors);

        gen_divisors_rec(1ULL, 0,
                         primes, exps, num_factors,
                         bf, cand, &cand_len);
    }

    /* Sort the candidate values */
    qsort(cand, cand_len, sizeof(seq_t), cmp_seq);

    /* Deduplicate and store in global cache */
    grow_cache(cand_len);
    seq_len = 0;
    for (size_t i = 0; i < cand_len; ++i)
    {
        if (seq_len == 0 || cmp_seq(&seq_cache[seq_len-1], &cand[i]) != 0)
            seq_cache[seq_len++] = cand[i];
    }

    free(cand);
    current_bmax = limit;
}

/* ------------------------------------------------------------ */
/* Make sure we have at least idx terms in the cache             */
static void ensure_terms(idx_t idx)
{
    if (seq_cache && seq_len >= (size_t)idx)
        return;

    /* Start with a moderate bound and double until we have enough terms */
    uint32_t bmax = current_bmax ? current_bmax : 1000;
    while (1)
    {
        compute_sequence(bmax);
        if (seq_len >= (size_t)idx)
            break;
        bmax <<= 1;                      /* double the bound */
    }
}

/* ------------------------------------------------------------ */
/* Public function required by the exercise */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not defined for 0, return 0 */
    ensure_terms(n);
    return seq_cache[(size_t)n - 1];
}
