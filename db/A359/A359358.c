/*
 * A359358  Let y be the integer partition with Heinz number n. Then a(n) is
 *          the size of the Young diagram of y after removing a rectangle of
 *          the same length as y and width equal to the smallest part of y.
 * https://oeis.org/A359358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A056239(n) - A001222(n) * A055396(n)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   build_primes_upto, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the size of a Young diagram after removing a rectangle
   of height equal to the number of parts and width equal to the
   smallest part. */

/* Required scalar types are supplied by the build system */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>

/* Include helpers that provide prime generation and prime factorisation */
#include "../utils/build_primes_upto.h"
#include "../utils/factor_u64.h"

static uint64_t *primes_list   = NULL;   /* primes up to current maximum */
static size_t   primes_count   = 0;
static idx_t    primes_max_lim = 0;      /* maximum number up to which primes are built */
static uint32_t *prime_idx     = NULL;   /* mapping value → 1‑based prime index */

/* Ensure that all primes up to at least 'limit' are known.  */
/* The function rebuilds the prime table if a larger limit is required. */
static void ensure_primes(idx_t limit)
{
    if (limit <= primes_max_lim)
        return;

    uint64_t *new_list = NULL;
    size_t    new_cnt  = 0;
    build_primes_upto(limit, &new_list, &new_cnt);

    free(primes_list);
    free(prime_idx);

    primes_list   = new_list;
    primes_count  = new_cnt;
    primes_max_lim = limit;

    /* Build a fast index lookup table for primes up to the limit. */
    prime_idx = (uint32_t *)calloc((size_t)(limit + 1), sizeof(uint32_t));
    if (prime_idx) {
        for (size_t i = 0; i < new_cnt; ++i) {
            uint64_t p = new_list[i];
            if (p <= (uint64_t)limit)
                prime_idx[(size_t)p] = (uint32_t)(i + 1);  /* 1‑based index */
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;               /* offset starts at 1 */

    if (n == 1)                        /* Heinz number 1 → empty partition */
        return (seq_t)0;

    /* Prepare primes up to n for mapping prime values → indices. */
    ensure_primes(n);

    /* Factorise n into distinct primes and their exponents. */
    uint64_t pf[64];
    uint64_t ex[64];
    size_t   fc = 0;
    factor_u64(n, pf, ex, &fc);

    if (fc == 0)                       /* shouldn't happen for n > 1 */
        return (seq_t)0;

    seq_t  sum_parts   = 0;            /* sum of parts in the partition */
    uint64_t total_parts = 0;          /* total number of parts (sum of exponents) */
    uint32_t min_idx   = UINT32_MAX;   /* smallest part (minimum prime index) */

    for (size_t i = 0; i < fc; ++i) {
        uint64_t p = pf[i];

        /* Get the 1‑based index of prime p. */
        uint32_t idx = 0;
        if (prime_idx && p <= (uint64_t)primes_max_lim)
            idx = prime_idx[(size_t)p];

        if (idx == 0) {                 /* fallback: binary search in primes_list */
            size_t lo = 0, hi = primes_count;
            while (lo < hi) {
                size_t mid = (lo + hi) >> 1;
                if (primes_list[mid] < p)
                    lo = mid + 1;
                else
                    hi = mid;
            }
            if (lo < primes_count && primes_list[lo] == p)
                idx = (uint32_t)(lo + 1);
        }

        if (idx == 0)                   /* should never happen */
            continue;

        sum_parts   += (seq_t)idx * (seq_t)ex[i];
        total_parts += ex[i];
        if (idx < min_idx)
            min_idx = idx;
    }

    /* a(n) = sum_of_parts - number_of_parts * smallest_part */
    return sum_parts - (seq_t)total_parts * (seq_t)min_idx;
}
