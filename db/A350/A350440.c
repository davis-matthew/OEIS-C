/*
 * A350440  a(n) is the number of integers that can be represented in a
 *          7-segment display by using only n segments (version A277116).
 * https://oeis.org/A350440
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 21, u32 n <= 42, u64 n <= 82, u128 n <= 164,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A350440: Number of integers that can be represented 
   on a 7‑segment display using exactly n segments.
   Recurrence (n > 13):
        a(n) = a(n-2) + a(n-3) + a(n-4) + 4*a(n-5) + 2*a(n-6) + a(n-7)
   Initial values:
        a(0)=0, a(1)=0, a(2)=1, a(3)=2, a(4)=3, a(5)=8,
        a(6)=12, a(7)=19, a(8)=33, a(9)=55, a(10)=103,
        a(11)=170, a(12)=297, a(13)=509.
*/
#include <stdlib.h>

static seq_t *cache = NULL;          /* pointer to the cached values */
static idx_t cache_len = 0;          /* number of entries that are valid   */
static idx_t cache_cap = 0;          /* capacity of the cache array         */

static void ensure_capacity(idx_t needed)
{
    if (needed <= cache_cap)
        return;

#if defined(__cplusplus)
    /* C++: use new[] / delete[] to create a class array. */
    seq_t *new_cache = new seq_t[needed];
    for (idx_t i = 0; i < cache_len; ++i)
        new_cache[i] = cache[i];
    delete[] cache;
    cache = new_cache;
#else
    /* C: use realloc on a malloc'ed block. */
    seq_t *tmp = (seq_t*)realloc(cache, (size_t)needed * sizeof(seq_t));
    if (!tmp)
        return; /* allocation failure – caller will get zero result */
    cache = tmp;
#endif
    cache_cap = needed;
}

seq_t a(idx_t n)
{
    /* If the value is already computed, return it immediately. */
    if (n < cache_len)
        return cache[n];

    /* Ensure sufficient capacity. */
    ensure_capacity(n + 1);

    /* Fill initial terms. These are valid for all n < 14. */
    static const seq_t base[14] = {
        0,0,1,2,3,8,12,19,33,55,103,170,297,509
    };
    if (cache_len < 14) {
        idx_t up_to = n < 14 ? n : 13;
        for (idx_t i = cache_len; i <= up_to; ++i)
            cache[i] = base[i];
        cache_len = up_to + 1;
    }

    /* Compute further terms using the recurrence. */
    if (cache_len <= n) {
        for (idx_t i = cache_len; i <= n; ++i) {
            /* i >= 14 because cache_len will be at least 14 after the above. */
            cache[i] =
                cache[i-2] +
                cache[i-3] +
                cache[i-4] +
                cache[i-5] * (seq_t)4 +
                cache[i-6] * (seq_t)2 +
                cache[i-7];
        }
        cache_len = n + 1;
    }

    return cache[n];
}
