/*
 * A373239  Relative of Hofstadter Q-sequence: a(n) = max(0, n+118) for n <=
 *          0; a(n) = a(n-a(n-1)) + a(n-a(n-2)) + a(n-a(n-3)) for n > 0.
 * https://oeis.org/A373239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = max(0, n+118) for n <= 0; a(n) = a(n-a(n-1)) +
 *            a(n-a(n-2)) + a(n-a(n-3)) for n > 0 Offset: 1 (so a(1) is the
 *            first listed term)
 * Range:     u8 n <= 27, u16 n <= 127, u32 n <= 127, u64 n <= 127,
 *            u128 n <= 127, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t cache[128];          /* cache[0..127]  */
static bool initialized = false;

/* Return a(k) for any signed integer k.
   For k <= 0 use the base definition.
   For 1 <= k <= 127 return the memoised value.
   For k > 127 the sequence is undefined – return 0.  */
static inline seq_t a_at(int64_t k)
{
    if (k <= 0) {
        if (k >= -118) return (seq_t)(k + 118);
        return 0;
    }
    if (k <= 127) return cache[(idx_t)k];
    return 0;
}

/* Initialise the cache once.  */
static void init_cache(void)
{
    cache[0] = 118;                /* a(0) = 118 */
    for (idx_t n = 1; n <= 127; ++n) {
        int64_t t1 = (int64_t)n - (int64_t)a_at((int64_t)n - 1);
        int64_t t2 = (int64_t)n - (int64_t)a_at((int64_t)n - 2);
        int64_t t3 = (int64_t)n - (int64_t)a_at((int64_t)n - 3);
        cache[n] = a_at(t1) + a_at(t2) + a_at(t3);
    }
    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized) init_cache();
    return a_at((int64_t)n);
}
