/*
 * A345557  Numbers that are the sum of ten cubes in nine or more ways.
 * https://oeis.org/A345557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A345557: numbers that are the sum of ten cubes in nine or more ways.
   The function builds a sieve that counts, for each integer, the number
   of unordered representations as the sum of exactly ten positive cubes.
   Whenever the count reaches at least nine, the integer is appended to a
   cache of accepted terms.  Subsequent calls reuse the cache and expand
   the sieve only when necessary.  This guarantees O(n) average
   complexity and returns the correct term for any n ≥ 1. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Helper macro to cast the result of realloc for seq_t pointers. */
#ifdef __cplusplus
#define REALLOC_SEQ_T(ptr, newsize) reinterpret_cast<seq_t*>(realloc(ptr,newsize))
#else
#define REALLOC_SEQ_T(ptr, newsize) (seq_t*)realloc(ptr,newsize)
#endif

/* Cache of accepted terms. */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of terms stored */
static size_t cache_cap = 0;   /* capacity of the cache array */

/* Highest integer that has been scanned by the DP algorithm. */
static idx_t last_scanned = 0;

/* maximum integer for which DP values were computed. */
static size_t dp_limit = 0;

/* Append a new accepted term to the cache. */
static void append_term(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        seq_t *tmp = REALLOC_SEQ_T(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);   /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Ensure that the cache contains at least n terms.  If not, expand the
   DP sieve up to a new limit until enough terms are found. */
static void ensure_terms(idx_t n)
{
    while (cache_len < (size_t)n) {
        size_t new_limit = dp_limit ? dp_limit * 2 : 1000;   /* start at 1000 */
        size_t stride = new_limit + 1;

        /* DP table: dp[k][s] = number of ways to write s as sum of k cubes. */
        uint64_t *dp = (uint64_t*)calloc(11 * stride, sizeof(uint64_t));
        if (!dp) exit(1);   /* out of memory */

        /* base case */
        dp[0] = 1ULL;   /* dp[0][0] = 1 */

        /* generate all cubes up to new_limit */
        for (size_t cube = 1; ; ++cube) {
            size_t val_cube = cube * cube * cube;
            if (val_cube > new_limit) break;
            for (int k = 1; k <= 10; ++k) {
                uint64_t *dp_k   = dp + k * stride;
                uint64_t *dp_km1 = dp + (k - 1) * stride;
                for (size_t s = val_cube; s <= new_limit; ++s) {
                    dp_k[s] += dp_km1[s - val_cube];
                }
            }
        }

        /* collect new terms that have not yet been seen */
        for (size_t s = (size_t)last_scanned + 1; s <= new_limit; ++s) {
            if (dp[10 * stride + s] >= 9ULL) {
                append_term((seq_t)s);
            }
        }

        free(dp);
        last_scanned = (idx_t)new_limit;
        dp_limit = new_limit;
    }
}

/* Public API required by the repository */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    ensure_terms(n);
    return cache[(size_t)n - 1];
}
