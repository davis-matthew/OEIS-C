/*
 * A350983  a(0)=1, a(1)=25, a(2)=865; a(n) = 35*(a(n-1)-a(n-2))+a(n-3).
 * https://oeis.org/A350983
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=25, a(2)=865; a(n) = 35*(a(n-1)-a(n-2))+a(n-3)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static size_t cache_cap = 0;

static void ensure_capacity(idx_t n)
{
    if ((size_t)(n + 1) <= cache_cap)
        return;
    size_t needed = (size_t)(n + 1);
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        return; /* allocation failed, leave cache unchanged */
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)25;
    if (n == 2) return (seq_t)865;

    ensure_capacity(n);
    if (cache_len < 3) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)25;
        cache[2] = (seq_t)865;
        cache_len = 3;
    }

    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t diff = cache[i - 1] - cache[i - 2];
        cache[i] = (seq_t)35 * diff + cache[i - 3];
    }
    cache_len = n + 1;
    return cache[n];
}
