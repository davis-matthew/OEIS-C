/*
 * A381279  a(1) = 1, for n > 1, a(n) = a(n - 1) / 2 if a(n - 1) is divisible
 *          by 2, otherwise a(n) = a(n - 1) + A000005(n - 1).
 * https://oeis.org/A381279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n > 1, a(n) = a(n-1)/2 if a(n-1) is even,
 *            otherwise a(n) = a(n-1) + d(n-1); where d(k) is the number of
 *            divisors of k (A000005)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/divisor_count_u64.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cached_max = 0;

static void ensure_cache(idx_t n)
{
    if (n >= cache_cap) {
        size_t new_cap = n + 1;
        if (new_cap < 4) new_cap = 4;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) exit(EXIT_FAILURE);
        if (cache) {
            for (size_t i = 0; i <= cached_max; ++i)
                new_cache[i] = cache[i];
            free(cache);
        }
        cache = new_cache;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* below the offset – result does not matter */

    ensure_cache(n);

    if (cached_max == 0) { /* first call: initialize a(1) */
        cache[1] = (seq_t)1;
        cached_max = 1;
    }

    for (idx_t k = cached_max + 1; k <= n; ++k) {
        seq_t prev = cache[k - 1];
        if ((prev & 1) == 0) {          /* even */
            cache[k] = prev >> 1;
        } else {                        /* odd */
            uint64_t d = divisor_count_u64((uint64_t)(k - 1));
            cache[k] = prev + (seq_t)d;
        }
    }

    cached_max = n;
    return cache[n];
}
