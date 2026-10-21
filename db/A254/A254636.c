/*
 * A254636  Numbers that cannot be represented as x*y + x + y, where x>=y>1.
 * https://oeis.org/A254636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A254636: Numbers that cannot be expressed as x*y + x + y with x>=y>1.
 * The algorithm tests each integer n; n is in the sequence iff n+1 has no
 * factorisation n+1 = a*b with a>=3 and b>=3.  Results are cached so
 * the function works independently of call order. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* Small primality test for idx_t; used only for the auxiliary check. */
static bool is_prime_idx(idx_t n) {
    if (n < 2) return false;
    if (n == 2) return true;
    if ((n & 1ULL) == 0) return false;
    for (idx_t d = 3; d * d <= n; d += 2) {
        if (n % d == 0) return false;
    }
    return true;
}

/* Return true iff n can be written as x*y + x + y with x>=y>1. */
static bool is_representable(idx_t n) {
    idx_t t = n + 1;                       /* (x+1)(y+1) */
    if (t <= 4) return false;              /* 1,2,3,4 give no valid factorisation */
    if (is_prime_idx(t)) return false;     /* prime → cannot be factored with >=3 */
    for (idx_t d = 3; d * d <= t; d++) {
        if (t % d == 0) {
            idx_t other = t / d;
            if (other >= 3) return true;   /* both factors >=3 */
        }
    }
    return false;
}

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_candidate = 0;

/* Grow the cache so it holds at least 'need' entries. */
static void grow_cache(size_t need) {
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    if (new_cap < need) new_cap = need;
    if (cache == NULL) {
        cache = seq_alloc(new_cap);
        if (cache == NULL) abort();
    } else {
        seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
        if (tmp == NULL) abort();
        cache = tmp;
    }
    cache_cap = new_cap;
}

seq_t a(idx_t n) {
    while (cache_len < (size_t)n) {
        if (!is_representable(next_candidate)) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)next_candidate;
        }
        next_candidate++;
    }
    return cache[n - 1];
}
