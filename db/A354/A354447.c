/*
 * A354447  Taxicab numbers (sums of 2 cubes in more than 1 way) which are
 *          products of four distinct primes.
 * https://oeis.org/A354447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A354447 consists of taxicab numbers (sums of two positive cubes
 *            in at least two distinct ways) that are square‑free and have
 *            exactly four distinct prime factors. The function below computes
 *            the sequence by progressively generating all sums of two cubes
 *            up to a cubic limit, counting duplicates, an...
 * Range:     u8 none, u16 none, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   count_distinct_primes, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/is_squarefree.h"
#include "../utils/count_distinct_primes.h"

/* compare function for qsort on uint64_t */
static int cmp_uint64(const void *p, const void *q)
{
    uint64_t a = *(const uint64_t *)p;
    uint64_t b = *(const uint64_t *)q;
    return (a < b) ? -1 : (a > b) ? 1 : 0;
}

/* generate all qualifying taxicab numbers up to cube limit 'limit'.
   The resulting array and its length are returned in *out and *len. */
static void generate_candidates(idx_t limit, seq_t **out, size_t *len)
{
    /* number of unordered pairs 1 <= a < b <= limit */
    size_t pair_cnt = (size_t)limit * (size_t)(limit - 1) / 2;

    uint64_t *sums = (uint64_t *)malloc(pair_cnt * sizeof(uint64_t));
    if (!sums) {
        *out = NULL;
        *len = 0;
        return;
    }

    size_t pos = 0;
    for (uint64_t a = 1; a < limit; ++a) {
        uint64_t cube_a = a * a * a;
        for (uint64_t b = a + 1; b <= limit; ++b) {
            sums[pos++] = cube_a + b * b * b;
        }
    }

    qsort(sums, pair_cnt, sizeof(uint64_t), cmp_uint64);

    /* collect sums that occur at least twice and fulfill the prime criteria */
    size_t cap = 128;
    seq_t *cand = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!cand) {
        free(sums);
        *out = NULL;
        *len = 0;
        return;
    }
    size_t clen = 0;

    for (size_t i = 0; i < pair_cnt; ) {
        uint64_t v = sums[i];
        size_t j = i + 1;
        while (j < pair_cnt && sums[j] == v) ++j;
        if (j - i >= 2) {                     /* at least two representations */
            if (is_squarefree((seq_t)v) &&
                count_distinct_primes(v) == 4) {
                if (clen == cap) {
                    cap <<= 1;
                    seq_t *tmp = (seq_t *)realloc(cand, cap * sizeof(seq_t));
                    if (!tmp) {  /* allocation failure, abort */
                        free(cand);
                        free(sums);
                        *out = NULL;
                        *len = 0;
                        return;
                    }
                    cand = tmp;
                }
                cand[clen++] = (seq_t)v;
            }
        }
        i = j;
    }

    free(sums);
    *out = cand;
    *len = clen;
}

/* static memory for memoization */
static seq_t *seq_cache   = NULL;  /* array of found terms */
static idx_t  seq_cache_len = 0;   /* number of terms stored */
static idx_t  current_limit  = 0;   /* current cube limit used */

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined; safety */

    /* initialise cache if first call */
    if (!seq_cache) {
        current_limit = 2000;            /* starting cube root limit */
        generate_candidates(current_limit, &seq_cache, &seq_cache_len);
    }

    /* extend the search until we have at least 'n' terms */
    while (seq_cache_len < n) {
        current_limit <<= 1;            /* double the limit */
        seq_t *new_cache = NULL;
        size_t new_len  = 0;
        generate_candidates(current_limit, &new_cache, &new_len);
        free(seq_cache);                /* discard old data */
        seq_cache      = new_cache;
        seq_cache_len  = (idx_t)new_len;
    }

    return seq_cache[n - 1];   /* 0‑based indexing in the array */
}
