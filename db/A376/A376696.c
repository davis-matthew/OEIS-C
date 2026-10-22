/*
 * A376696  a(n) = Sum_{k=0..floor(n/3)} binomial(n-3*k,floor(k/3)).
 * https://oeis.org/A376696
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+a(n-10), with a(0)=1, a(1)=1, a(2)=1, a(3)=2,
 *            a(4)=2, a(5)=2, a(6)=3, a(7)=3, a(8)=3, a(9)=3
 * Range:     u8 n <= 32, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function is memoised so it works in any call order. */

#include <stddef.h>
#include "../utils/ensure_cache.h"

static const seq_t base_vals[10] = {
    (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)2,
    (seq_t)2, (seq_t)3, (seq_t)3, (seq_t)3, (seq_t)3
};

static seq_t *cache = NULL;        /* memoised values */
static size_t cap   = 0;           /* current allocated capacity */
static idx_t  computed = 0;        /* number of values stored (next free index) */

seq_t a(idx_t n)
{
    /* Ensure we have enough storage for indices up to n. */
    ensure_cache((idx_t)(n + 1), &cache, &cap);

    /* Compute any values that are missing. */
    for (idx_t i = computed; i <= n; ++i) {
        if (i < 10) {
            cache[i] = base_vals[i];
        } else {
            /* recurrence a(i) = a(i-1) + a(i-10) */
            cache[i] = cache[i - 1] + cache[i - 10];
        }
    }

    /* Update the number of computed values. */
    computed = n + 1;

    return cache[n];
}
