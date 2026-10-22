/*
 * A393860  Prime powers q such that q-1 is squarefree.
 * https://oeis.org/A393860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   factor_u64, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A393860: prime powers q such that q‑1 is squarefree. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/is_squarefree.h"

static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap   = 0;
static idx_t  next_q      = 2;

static void grow_cache(size_t need) {
    if (cache_cap < need) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < need) new_cap = need;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache    = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset is 1, but safe guard */
    while (cache_size < n) {
        uint64_t q64 = (uint64_t)next_q;
        uint64_t pr[10], ex[10];
        size_t cnt;

        factor_u64(q64, pr, ex, &cnt);   /* factorize q64 */

        if (cnt == 1) {            /* q64 is a prime power */
            seq_t v        = (seq_t)q64;
            seq_t v_minus  = v - 1;     /* q-1 */
            if (is_squarefree(v_minus)) {
                grow_cache(cache_size + 1);
                cache[cache_size++] = v;
            }
        }
        ++next_q;
    }
    return cache[n - 1];
}
