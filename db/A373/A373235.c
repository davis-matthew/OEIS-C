/*
 * A373235  Relative of Hofstadter Q-sequence: a(n) = max(0, n+2087) for n <=
 *          0; a(n) = a(n-a(n-1)) + a(n-a(n-2)) + a(n-a(n-3)) for n > 0.
 * https://oeis.org/A373235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = a(n-a(n-1)) + a(n-a(n-2)) + a(n-a(n-3)) a(n) = max(0,
 *            n+2087) for n <= 0
 * Range:     u8 n <= 1, u16 n <= 2341, u32 n <= 2341, u64 n <= 2341,
 *            u128 n <= 2341, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses memoised recursion.  For positive indices
   the value is cached in a dynamic array.  For non‑positive indices a
   simple closed form is used. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>

static seq_t *cache      = NULL;   /* cache[n] holds a(n) for n >= 1           */
static bool   *cached    = NULL;   /* true if cache[n] is valid               */
static size_t  cap       = 0;      /* capacity of cache and cached arrays     */

/* Ensure that the cache can hold indices up to 'n' (inclusive).  The
   index 0 is unused because the sequence is defined from n = 1.          */
static void ensure_cache(idx_t n)
{
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap ? cap * 2 : 16;
        if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;

        cache  = (seq_t*)   realloc(cache,  new_cap * sizeof(seq_t));
        cached = (bool*)    realloc(cached, new_cap * sizeof(bool));

        for (size_t i = cap; i < new_cap; ++i) {
            cache[i]  = 0;
            cached[i] = false;
        }
        cap = new_cap;
    }
}

/* Forward declaration of the signed recursion function. */
static seq_t get_a_signed(int64_t idx);

/* Compute a(n) for positive n, using memoisation. */
static seq_t get_a_pos(idx_t n)
{
    ensure_cache(n);
    if (cached[n]) return cache[n];

    /* Values of a(n-1), a(n-2), a(n-3) (may be zero or positive).   */
    seq_t prev1 = get_a_signed((int64_t)n - 1);
    seq_t prev2 = get_a_signed((int64_t)n - 2);
    seq_t prev3 = get_a_signed((int64_t)n - 3);

    /* Indices for the recursive calls: n - a(n-1) etc.               */
    seq_t term1 = get_a_signed((int64_t)n - (int64_t)prev1);
    seq_t term2 = get_a_signed((int64_t)n - (int64_t)prev2);
    seq_t term3 = get_a_signed((int64_t)n - (int64_t)prev3);

    seq_t val = term1 + term2 + term3;

    cache[n]   = val;
    cached[n]  = true;
    return val;
}

/* Compute a(n) for signed indices.  For n <= 0 the closed form is
   used; for n > 0 the memoised routine is called.     */
static seq_t get_a_signed(int64_t idx)
{
    if (idx <= 0) {
        int64_t tmp = idx + 2087;
        return (tmp > 0) ? (seq_t)tmp : (seq_t)0;
    }
    return get_a_pos((idx_t)idx);
}

/* Public entry point – n must be >= 1. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n==0, safe guard       */
    return get_a_signed((int64_t)n);
}
