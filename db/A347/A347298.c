/*
 * A347298  Numbers that contain an even digit d immediately followed by a
 *          digit <= d.
 * https://oeis.org/A347298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A347298: Numbers that contain an even digit d immediately followed by a digit <= d. */
#include <stdbool.h>
#include <stdlib.h>

/* Check whether x satisfies the property.  x may be larger than the range of seq_t. */
static bool satisfies(uint64_t x)
{
    while (x >= 10) {
        uint64_t e = x % 10;        /* rightmost digit */
        uint64_t d = (x / 10) % 10; /* preceding digit */
        if ((d % 2 == 0) && (e <= d))
            return true;
        x /= 10;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* Cache of already-computed terms.  Terms are stored in the order they
       appear in the sequence; cache[0] is a(1). */
    static seq_t *cache = NULL;
    static idx_t size  = 0;       /* number of terms currently cached */
    static uint64_t cur = 0;      /* last number examined (may exceed seq_t range) */

    if (n == 0)
        return 0;                 /* offset behaviour is irrelevant */

    /* Grow the cache if necessary. */
    if (n > size) {
        idx_t new_cap = size ? (idx_t)(size * 2) : 8;
        if (new_cap < n)
            new_cap = n;

        if (cache == NULL) {
            cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        } else {
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) {
                /* Allocation failed; abort with some safe value. */
                abort();
            }
            cache = tmp;
        }

        /* Compute new terms and append them to the cache. */
        for (idx_t i = size; i < n; ++i) {
            do {
                ++cur;
            } while (!satisfies(cur));
            cache[(size_t)i] = (seq_t)cur;
        }
        size = n;
    }

    return cache[(size_t)(n - 1)];
}
