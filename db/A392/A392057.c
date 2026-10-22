/*
 * A392057  a(0)=0, a(n) = wt(a(n-1)) + wt(n), where wt(x) is the binary
 *          weight of x, A000120.
 * https://oeis.org/A392057
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = wt(a(n-1)) + wt(n) with a(0) = 0, where wt(x) is the
 *            binary weight (popcount) of x
 * Range:     u8 n <= 9216, u16 n <= 9216, u32 n <= 9216, u64 n <= 9216,
 *            u128 n <= 9216, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Popcount: count 1-bits of a 64‑bit unsigned integer. */
static inline int popcount_u64(uint64_t x) {
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_popcountll(x);
#else
    unsigned c = 0;
    while (x) {
        c++;
        x &= x - 1;
    }
    return c;
#endif
}

/* Popcount: generic for any seq_t that supports '&' and '>>'. */
static inline int popcount_seq(seq_t x) {
    int c = 0;
    while (x != 0) {
        c += (int)(x & 1u);
        x >>= 1;
    }
    return c;
}

/* Cached values of a(n). */
static seq_t *cache = NULL;
static size_t cache_len = 0;     /* number of elements currently stored */

/* Compute a(n) with memoisation. */
seq_t a(idx_t n) {
    size_t target = (size_t)n;   /* assume n fits into size_t for the test range */

    if (cache_len <= target) {
        size_t new_len = target + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;      /* allocation failure: return 0 */
        cache = tmp;

        if (cache_len == 0) {    /* initialise a(0) */
            cache[0] = 0;
            cache_len = 1;
        }

        for (size_t i = cache_len; i <= target; ++i) {
            seq_t prev = cache[i - 1];
            int pop_prev = popcount_seq(prev);
            uint64_t idx = (uint64_t)i;
            int pop_idx  = popcount_u64(idx);
            cache[i] = (seq_t)(pop_prev + pop_idx);
        }
        cache_len = new_len;
    }
    return cache[target];
}
