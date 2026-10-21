/*
 * A258169  a(n) = a(n-1)^4/a(n-2) with a(0)=1, a(1)=2.
 * https://oeis.org/A258169
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)^4 / a(n-2), a(0)=1, a(1)=2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   big_pow, seq_alloc_new_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_realloc.h"
#include <stddef.h>

static seq_t *exp_cache   = NULL;   /* store exponents e_n */
static seq_t *seq_cache   = NULL;   /* store a_n values   */
static size_t exp_cap    = 0, seq_cap = 0;
static size_t exp_size   = 0, seq_size = 0;

/* Ensure a cache array has room for at least 'needed' elements. */
static void grow_cache(seq_t **cache, size_t *cap, size_t needed)
{
    if (*cap >= needed) return;
    size_t new_cap = (*cap) ? (*cap) * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;
    if (*cache == NULL)
        *cache = seq_alloc_new_array(new_cap);
    else
        *cache = seq_realloc(*cache, *cap, new_cap);
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    /* initialise caches on first call */
    if (exp_cache == NULL) {
        grow_cache(&exp_cache, &exp_cap, 2);
        grow_cache(&seq_cache, &seq_cap, 2);
        exp_cache[0] = (seq_t)0;
        exp_cache[1] = (seq_t)1;
        seq_cache[0] = (seq_t)1;
        seq_cache[1] = (seq_t)2;
        exp_size = seq_size = 2;
    }

    if ((size_t)n < exp_size)
        return seq_cache[n];

    size_t needed = (size_t)n + 1;
    grow_cache(&exp_cache, &exp_cap, needed);
    grow_cache(&seq_cache, &seq_cap, needed);

    for (size_t i = exp_size; i < needed; ++i) {
        seq_t e_prev   = exp_cache[i - 1];
        seq_t e_prev2  = exp_cache[i - 2];
        seq_t e        = e_prev * (seq_t)4 - e_prev2;   /* e_n = 4*e_{n-1} - e_{n-2} */
        exp_cache[i]   = e;
        seq_cache[i]   = big_pow((seq_t)2, e);          /* a_n = 2^e_n */
    }
    exp_size = seq_size = needed;
    return seq_cache[n];
}
