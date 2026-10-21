/*
 * A253457  Indices of centered hexagonal numbers (A003215) which are also
 *          centered heptagonal numbers (A069099).
 * https://oeis.org/A253457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 27*a(n-1) - 27*a(n-2) + a(n-3) Initial values: a(1) = 1,
 *            a(2) = 14, a(3) = 351
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#define MAX_CACHE 10000   /* sufficient for n up to ~10000 */

static seq_t cache[MAX_CACHE + 1];
static idx_t max_computed = 0;

/* ensure the three previous values are set up to at least idx */
static void ensure_base(idx_t idx) {
    if (max_computed < 1) {
        cache[1] = (seq_t)1;
        max_computed = 1;
    }
    if (max_computed < 2) {
        cache[2] = (seq_t)14;
        max_computed = 2;
    }
    if (max_computed < 3) {
        cache[3] = (seq_t)351;
        max_computed = 3;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;            /* out of range, not used */

    /* compute up to the requested index, caching results when possible */
    if (n <= MAX_CACHE) {
        ensure_base(n);
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            seq_t val = 27 * cache[i - 1];
            val -= 27 * cache[i - 2];
            val += cache[i - 3];
            cache[i] = val;
        }
        max_computed = n;
        return cache[n];
    }

    /* n larger than the cache size: compute first up to MAX_CACHE,
       then continue without caching beyond that point. */
    if (max_computed < MAX_CACHE) {
        for (idx_t i = max_computed + 1; i <= MAX_CACHE; ++i) {
            seq_t val = 27 * cache[i - 1];
            val -= 27 * cache[i - 2];
            val += cache[i - 3];
            cache[i] = val;
        }
        max_computed = MAX_CACHE;
    }

    /* Take the last three computed cached values */
    seq_t prev3 = cache[max_computed - 2];
    seq_t prev2 = cache[max_computed - 1];
    seq_t prev1 = cache[max_computed];

    seq_t val;
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        val = 27 * prev1;
        val -= 27 * prev2;
        val += prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = val;
    }
    return val;
}
