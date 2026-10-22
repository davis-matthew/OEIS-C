/*
 * A322163  Minimal number of steps needed to get from n to 1, where for n > 1
 *          the next step is to either n-1 or max(a,b) for any a > 1 and b > 1
 *          such that ab=n.
 * https://oeis.org/A322163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9521, u16 n <= 9521, u32 n <= 9521, u64 n <= 9521,
 *            u128 n <= 9521, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=0;
   a(n)=1+min( a(n-1), min_{a*b=n, a>1,b>1} a(max(a,b)) ) */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* External type definitions.  The build supplies them. */
#ifndef seq_t
/* seq_t will be defined by the build system (uint8_t, uint32_t, etc. or
   an arbitrary‑precision integer class). */
#endif

/* Cache of computed values.  The cache is expanded lazily. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated slots (index 0 unused) */
static idx_t  computed_max = 0; /* largest index already computed */

/* Integer square root: floor(sqrt(n)) using 128‑bit multiplication
   to avoid overflow.  Works for idx_t up to 64 bits. */
static idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n;
    idx_t res = 0;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        /* mid*mid can overflow 64 bits; use 128 bits for comparison. */
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= n) {
            res = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break; /* guard against underflow */
            hi = mid - 1;
        }
    }
    return res;
}

/* Ensure the cache has room for indices <= needed-1.  Capacity is
   doubled each time to keep amortised O(1) reallocation.  The returned
   pointer may move; callers must not keep stale pointers. */
static void ensure_cache_capacity(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 1;
    if (new_cap < needed) new_cap = needed;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr) {
        /* In a real system you might exit or handle the error.  For
           the purposes of this exercise we simply abort. */
        abort();
    }
    cache = new_ptr;
    cache_cap = new_cap;
}

/* Compute a(n).  The function is memoised; calls may come in any order. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined, but safe */

    /* Ensure the cache can hold index n.  The cache index 0 is unused. */
    ensure_cache_capacity((size_t)n + 1);

    /* Compute DP values from the first uncomputed index up to n. */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        if (i == 1) {
            cache[1] = (seq_t)0;
            continue;
        }

        /* Candidate via decrement */
        seq_t best = cache[i - 1] + (seq_t)1;

        /* Candidates via factorisation.  We only need factors a>1. */
        idx_t sqrt_i = isqrt_u64(i);
        for (idx_t a_factor = 2; a_factor <= sqrt_i; ++a_factor) {
            if (i % a_factor != 0) continue;
            idx_t b_factor = i / a_factor;   /* b_factor >= a_factor > 1 */
            seq_t cand = cache[b_factor] + (seq_t)1;
            if (cand < best) best = cand;
        }

        cache[i] = best;
    }

    if ((idx_t)n > computed_max) computed_max = n;
    return cache[n];
}
