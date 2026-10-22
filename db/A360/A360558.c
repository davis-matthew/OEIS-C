/*
 * A360558  Numbers whose multiset of prime factors (or indices, see A112798)
 *          has more adjacent equalities (or parts that have appeared before)
 *          than distinct parts.
 * https://oeis.org/A360558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest numbers whose total number of prime factors (Ω)
 *            exceeds twice the number of distinct prime factors (ω), i.e.
 *            Ω(m) > 2·ω(m). The sequence is then the increasing list of such
 *            numbers
 * Range:     u8 n <= 24, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   factor_u64, seq_alloc_new_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;            /* cached sequence values */
static size_t cache_cap = 0;           /* capacity of cache */
static size_t cache_cnt = 0;           /* number of values stored */
static uint64_t last_checked = 1;      /* last integer examined */

/* Return true iff m satisfies Ω(m) > 2·ω(m) */
static bool matches(uint64_t m)
{
    uint64_t primes[64], exps[64];
    size_t cnt;
    factor_u64(m, primes, exps, &cnt);

    size_t omega = 0;
    for (size_t i = 0; i < cnt; ++i)
        omega += (size_t)exps[i];

    return omega > 2 * cnt;
}

seq_t a(idx_t n)
{
    /* Ensure cache is initialised */
    if (cache == NULL) {
        cache_cap = 256;                     /* initial capacity */
        cache = seq_alloc_new_array(cache_cap);
        cache_cnt = 0;
    }

    /* Return cached value if available */
    if ((size_t)n <= cache_cnt)
        return cache[(size_t)n - 1];

    /* Extend cache until we have n terms */
    uint64_t candidate = last_checked + 1;

    while (cache_cnt < (size_t)n) {
        if (matches(candidate)) {
            /* Grow cache if needed */
            if (cache_cnt == cache_cap) {
                size_t new_cap = cache_cap * 2;
                cache = seq_realloc(cache, cache_cap, new_cap);
                cache_cap = new_cap;
            }
            cache[cache_cnt++] = (seq_t)candidate;
        }
        ++candidate;
    }

    last_checked = candidate - 1;
    return cache[(size_t)n - 1];
}
