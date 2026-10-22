/*
 * A359163  Numbers of the form 4u+3 with an even number of prime factors
 *          (counted with multiplicity).
 * https://oeis.org/A359163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   count_prime_factors_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A359163: numbers of the form 4u+3 with an even number of prime factors
   (counted with multiplicity).  This implementation finds terms sequentially,
   caching them so that calls in arbitrary order run in O(n) time for the
   n‑th term when n ≈ 10⁴. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/count_prime_factors_uint64.h"

/* --- cache for the sequence ---------------------------------------- */
static seq_t *cache      = NULL; /* array of returned terms          */
static size_t cache_len  = 0;    /* number of terms currently stored */
static size_t cache_cap  = 0;    /* allocated capacity                */

static void ensure_cache_capacity(size_t needed)
{
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < needed)
            new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort(); /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }
}

static void cache_append(seq_t value)
{
    ensure_cache_capacity(cache_len + 1);
    cache[cache_len++] = value;
}

/* --- main sequence routine ------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* undefined, but harmless */

    /* start searching from the first 4u+3 integer (3) */
    static uint64_t next_candidate = 3;

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        /* count prime factors of the candidate */
        size_t phi = count_prime_factors_uint64(next_candidate);
        if ((phi & 1ULL) == 0) {                /* even number of factors */
            cache_append((seq_t)next_candidate);
        }
        next_candidate += 4;                    /* next 4u+3 number */
    }

    return cache[n - 1];
}
