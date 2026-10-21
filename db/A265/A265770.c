/*
 * A265770  Numerators of primes-only best approximates (POBAs) to 6; see
 *          Comments.
 * https://oeis.org/A265770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   is_prime_big, min64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of the numerators of the “primes‑only
   best approximates” of 6.  
   For each prime denominator q (in increasing order) we find the prime
   numerator(s) p that minimize |6 – p/q|.  
   An approximation p/q is stored only if its error
   |6 – p/q| = |6q – p|/q is strictly smaller than the error of all
   previously considered denominators.  If two primes are equidistant from
   6q both are stored, in the order of the larger prime first, as in the
   OEIS listing.  The function uses memoisation so that the nth term can
   be obtained in O(1) after a short precomputation. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/min64.h"

/* static state for memoisation */
static seq_t *cache = NULL;     /* cached numerators */
static size_t cache_cap = 0;
static size_t cache_len = 0;

static uint64_t last_den_candidate = 1;  /* last integer tried for a prime denominator */
static uint64_t best_q = 1;              /* denominator that produced the best error so far */
static uint64_t best_diff = (uint64_t)-1;/* |6q – p| of the best error so far */

/* ------------------------------------------------------------------ */
/* Grow cache array to hold at least 'needed' elements                */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* Generate terms up to index n (inclusive).  Caller must call this
   before accessing cache[n-1]. */
static void ensure_n_terms(idx_t n)
{
    /* continue generating until we have n terms */
    while (cache_len < (size_t)n) {
        /* 1. find the next prime denominator q */
        uint64_t q;
        for (;;) {
            ++last_den_candidate;
            if (is_prime_big((seq_t)last_den_candidate))
                break;
        }
        q = last_den_candidate;

        /* 2. compute 6*q */
        uint64_t base = 6 * q;               /* 6*q fits in uint64_t for n<=1e4 */

        /* 3. nearest primes below and above base */
        uint64_t p_lower = base;
        for (;;) {
            if (is_prime_big((seq_t)p_lower))
                break;
            --p_lower;
        }

        uint64_t p_upper = base + 1;
        for (;;) {
            if (is_prime_big((seq_t)p_upper))
                break;
            ++p_upper;
        }

        /* 4. minimal distance to base */
        uint64_t dist_lower = base - p_lower;
        uint64_t dist_upper = p_upper - base;
        uint64_t dist_min   = min64(dist_lower, dist_upper);

        /* 5. decide if this denominator produces a better error than all
           previous ones */
        if ((unsigned __int128)dist_min * best_q <
            (unsigned __int128)best_diff * q) {

            /* 6. store the approximations (upper then lower if equal) */
            grow_cache(cache_len + 2);
            if (dist_lower < dist_upper) {           /* nearer below */
                cache[cache_len++] = (seq_t)p_lower;
            } else if (dist_upper < dist_lower) {    /* nearer above */
                cache[cache_len++] = (seq_t)p_upper;
            } else {                                 /* equal distances */
                cache[cache_len++] = (seq_t)p_upper;
                cache[cache_len++] = (seq_t)p_lower;
            }

            /* 7. update best error information */
            best_diff = dist_min;
            best_q    = q;
        }
    }
}

/* ------------------------------------------------------------------ */
/* Public entry point required by the build system */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;              /* undefined; protect against UB */
    ensure_n_terms(n);
    return cache[(size_t)(n - 1)];
}
