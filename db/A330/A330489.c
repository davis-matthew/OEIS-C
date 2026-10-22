/*
 * A330489  a(n) is equal to a(n-1) plus (a(n-1) written in base n but
 *          interpreted in base n+1), with a(1)=1.
 * https://oeis.org/A330489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = a(n-1) + (a(n-1) written in base n but interpreted in
 *            base n+1), with a(1) = 1
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 29, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   reinterpret_in_new_base
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation caches computed terms up to a fixed maximum index. */

#include <stdbool.h>
#include "../utils/reinterpret_in_new_base.h"

#define MAX_N 20000  /* sufficient for n up to about 10000 */

static seq_t seq_cache[MAX_N + 1];
static bool computed[MAX_N + 1];
static idx_t cache_max = 0;

/* Compute a(n) for the OEIS A330489 sequence. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for n < 1, but return 0 for safety */

    if (n > MAX_N) {
        /* In practice n will not exceed MAX_N; you can extend the cache if needed. */
        return 0;
    }

    if (cache_max == 0) {
        seq_cache[1] = 1;
        computed[1]  = true;
        cache_max = 1;
    }

    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t prev = seq_cache[i - 1];
        seq_t inc  = reinterpret_in_new_base(prev, i);
        seq_cache[i] = prev + inc;
        computed[i] = true;
    }
    cache_max = n;
    return seq_cache[n];
}
