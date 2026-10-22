/*
 * A360719  a(n) is the largest k < A126706(n) such that rad(k) =
 *          rad(A126706(n)), where rad(n) = A007947(n).
 * https://oeis.org/A360719
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest k < A126706(n) such that rad(k) = rad(A126706(n))
 * Range:     u8 n <= 99, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_distinct_primes, factor_u64, rad, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../utils/rad.h"
#include "../utils/count_distinct_primes.h"
#include "../utils/factor_u64.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static seq_t *lastRad = NULL;
static size_t lastRad_size = 0;

/* Extend the radical table so that index rad is valid. */
static void ensure_rad_size(idx_t rad)
{
    if (rad < lastRad_size) return;
    size_t new_size = rad + 1;
    seq_t *tmp = (seq_t *)realloc(lastRad, new_size * sizeof(seq_t));
    if (!tmp) abort();                       /* out of memory */
    memset(tmp + lastRad_size, 0,
           (new_size - lastRad_size) * sizeof(seq_t));
    lastRad      = tmp;
    lastRad_size = new_size;
}

/* Cache of a(n) values.  seq_cache[i] holds a(i+1). */
static seq_t *seq_cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;

/* Generate terms until we have at least n of them stored. */
static void ensure_cache(size_t n)
{
    static uint64_t cur = 1;                  /* last integer examined */

    while (cache_size < n) {
        ++cur;                                 /* examine next integer */

        /* Radical of cur and number of distinct primes. */
        idx_t   rad_idx = (idx_t)rad(cur);
        int     distinct = count_distinct_primes(cur);

        /* Determine if cur is non‑squarefree. */
        bool    repeat = false;
        uint64_t primes[64];
        uint64_t exps[64];
        size_t   cnt;
        factor_u64(cur, primes, exps, &cnt);
        for (size_t i = 0; i < cnt; ++i)
            if (exps[i] > 1) { repeat = true; break; }

        /* Previous number with the same radical. */
        ensure_rad_size(rad_idx);
        seq_t prev = lastRad[rad_idx];
        lastRad[rad_idx] = cur;                /* record cur as the latest */

        /* cur qualifies as a term of A126706? */
        if (distinct >= 2 && repeat) {
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = seq_realloc(seq_cache, cache_cap, new_cap);
                if (!tmp) abort();
                seq_cache = tmp;
                cache_cap = new_cap;
            }
            seq_cache[cache_size++] = prev;     /* store a(n) */
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* n < 1 not needed for OEIS offset 1 */

    if (!seq_cache) {
        cache_cap = 16;
        seq_cache = seq_alloc(cache_cap);
    }

    ensure_cache((size_t)n);
    return seq_cache[n-1];
}
