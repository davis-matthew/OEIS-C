/*
 * A270039  a(n) = Smallest m >= 7 containing no threes when represented in
 *          any base from 4 through n.
 * https://oeis.org/A270039
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   a(n) is the smallest integer m ≥ 7 that contains no digit '3' in
 *            its * representation in any base from 4 through n
 * Range:     u8 n <= 36, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Checks whether val has digit 3 in its representation in every base
 * from 4 up to max_base. Only bases <= val are relevant. */
static bool passes(seq_t val, idx_t max_base)
{
    seq_t val_copy = val;
    for (idx_t base = 4; base <= max_base; ++base) {
        seq_t base_t = (seq_t)base;
        if (base_t > val_copy)                /* base larger than value -> single digit >3 */
            continue;
        seq_t tmp = val_copy;
        while (tmp) {
            if ((tmp % base_t) == 3)
                return false;
            tmp /= base_t;
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n < 4)               /* offset is 4; below that behaviour is irrelevant */
        return (seq_t)0;

    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!new_cache)
            return (seq_t)0;          /* allocation failure, unlikely */
        if (cache) {
            memcpy(new_cache, cache, cache_size * sizeof(seq_t));
            free(cache);
        } else {
            memset(new_cache, 0, new_size * sizeof(seq_t));
        }

        /* Compute missing entries up to n. */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i < 4)
                continue;
            seq_t candidate = (i == 4) ? (seq_t)7 : new_cache[i - 1];
            while (!passes(candidate, i))
                candidate++;
            new_cache[i] = candidate;
        }

        cache = new_cache;
        cache_size = new_size;
    }

    return cache[n];
}
