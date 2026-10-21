/*
 * A279142  Binary representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 137", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A279142
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = -10*a(n-1)+1000*a(n-3)+10000*a(n-4)+a(n-8)+10*a(n-9)-1000*a(n-11)-10000*a(n-12)
   for n > 15.  Initial values: a(0)=1, a(1)=0, a(2)=101, a(3)=0, a(4)=10101, a(5)=0,
   a(6)=1010101, a(7)=0, a(8)=101110101, a(9)=10100000, a(10)=10000010101,
   a(11)=1110000000, a(12)=1010101110101, a(13)=10100000, a(14)=101010000010101,
   a(15)=1110000000. */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* initial 16 terms, stored as literals.  Truncation is allowed for
 * implementations where seq_t is smaller than 64 bits; values that
 * do not fit are not verified. */
static const seq_t init[16] = {
    (seq_t)1ULL,
    (seq_t)0ULL,
    (seq_t)101ULL,
    (seq_t)0ULL,
    (seq_t)10101ULL,
    (seq_t)0ULL,
    (seq_t)1010101ULL,
    (seq_t)0ULL,
    (seq_t)101110101ULL,
    (seq_t)10100000ULL,
    (seq_t)10000010101ULL,
    (seq_t)1110000000ULL,
    (seq_t)1010101110101ULL,
    (seq_t)10100000ULL,
    (seq_t)101010000010101ULL,
    (seq_t)1110000000ULL
};

/* simple dynamic cache of computed terms */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t  cache_len = 0;

/* Ensure cache has space for terms up to n (inclusive). */
static void
ensure_cache(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (need <= cache_cap)
        return;
    seq_t *new_cache = (seq_t *)malloc(need * sizeof(seq_t));
    if (!new_cache)
        return;  /* allocation failed – leave cache empty */
    if (cache)
        memcpy(new_cache, cache, cache_len * sizeof(seq_t));
    free(cache);
    cache = new_cache;
    cache_cap = need;
}

/* Compute all terms from cache_len up to n and store them in cache. */
static void
compute_terms(idx_t n)
{
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        if (i < 16) {
            cache[i] = init[i];
            continue;
        }
        /* recurrence */
        seq_t pos = 0;
        seq_t neg = 0;

        pos += cache[i - 3] * (seq_t)1000ULL;
        pos += cache[i - 4] * (seq_t)10000ULL;
        pos += cache[i - 8];
        pos += cache[i - 9] * (seq_t)10ULL;

        neg += cache[i - 1] * (seq_t)10ULL;
        neg += cache[i - 11] * (seq_t)1000ULL;
        neg += cache[i - 12] * (seq_t)10000ULL;

        cache[i] = pos - neg;
    }
    cache_len = (size_t)n + 1;
}

/* Public interface required by the suite */
seq_t
a(idx_t n)
{
    if (!cache)
        ensure_cache(n);
    else
        ensure_cache(n);
    compute_terms(n);
    return cache[(size_t)n];
}
