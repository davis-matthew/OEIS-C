/*
 * A292351  a(1) = a(2) = 1, a(3) = 3, a(4) = a(5) = 4; a(n) = a(n-a(n-1)) +
 *          a(n-a(n-2)) + a(n-a(n-3)) for n > 5.
 * https://oeis.org/A292351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-a(n-1)) + a(n-a(n-2)) + a(n-a(n-3)), with initial
 *            terms a(1)=1, a(2)=1, a(3)=3, a(4)=4, a(5)=4
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

/* static storage for memoised terms */
static seq_t *cache = NULL;          /* allocated cache */
static idx_t cache_alloc = 0;        /* allocated capacity */
static idx_t cache_size  = 0;        /* number of terms currently stored */

/* ensure that cache has room for at least needed terms */
static void ensure_capacity(idx_t needed)
{
    if (needed <= cache_alloc)
        return;
    idx_t new_cap = needed < 16 ? 16 : needed;
    while (new_cap < needed)
        new_cap <<= 1;          /* double until big enough */
    seq_t *new_cache = alloc_seq_array(new_cap);
    if (!new_cache)
        return; /* allocation failed – silently abort further growth */
    if (cache) {
        /* copy existing values */
        for (idx_t i = 0; i < cache_size; ++i)
            new_cache[i] = cache[i];
        free_seq_array(cache);
    }
    cache = new_cache;
    cache_alloc = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* undefined for indices < 1 */

    ensure_capacity(n);

    /* compute base terms if not yet present */
    if (cache_size < 5) {
        idx_t end = n < 5 ? n : 5;
        for (idx_t i = cache_size + 1; i <= end; ++i) {
            switch (i) {
                case 1: cache[i - 1] = 1; break;
                case 2: cache[i - 1] = 1; break;
                case 3: cache[i - 1] = 3; break;
                case 4: cache[i - 1] = 4; break;
                case 5: cache[i - 1] = 4; break;
            }
        }
        cache_size = end;
    }

    /* compute remaining terms up to n */
    if (cache_size < n) {
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            if (i <= 5) {
                /* already handled above – keep for completeness */
                switch (i) {
                    case 1: cache[i - 1] = 1; break;
                    case 2: cache[i - 1] = 1; break;
                    case 3: cache[i - 1] = 3; break;
                    case 4: cache[i - 1] = 4; break;
                    case 5: cache[i - 1] = 4; break;
                }
            } else {
                seq_t a1 = cache[i - 2]; /* a(i-1) */
                seq_t a2 = cache[i - 3]; /* a(i-2) */
                seq_t a3 = cache[i - 4]; /* a(i-3) */
                idx_t pos1 = i - (idx_t)a1;
                idx_t pos2 = i - (idx_t)a2;
                idx_t pos3 = i - (idx_t)a3;
                cache[i - 1] =
                    cache[pos1 - 1] + cache[pos2 - 1] + cache[pos3 - 1];
            }
        }
        cache_size = n;
    }

    return cache[n - 1];
}
