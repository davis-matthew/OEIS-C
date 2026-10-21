/*
 * A296695  Numbers whose base-4 digits d(m), d(m-1), ... d(0) have #(rises) >
 *          #(falls); see Comments.
 * https://oeis.org/A296695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A296695: a(n) is the nth natural number whose
   base‑4 digits contain more rises than falls.  The routine uses
   linear search with an incremental cache; no closed formula is known
   for this sequence. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Helper: test whether a number has more rises than falls in its
   base‑4 representation.  It works for any unsigned seq_t type. */
static bool is_good(seq_t x)
{
    if (x == 0)
        return false;

    uint8_t digits[256];   /* enough for any value up to 2^512 */
    int len = 0;
    const seq_t four = (seq_t)4;

    /* Extract base‑4 digits (least significant first) */
    while (x > 0) {
        digits[len++] = (uint8_t)(x % four);
        x /= four;
    }

    /* Reverse to most significant first */
    for (int i = 0; i < len / 2; ++i) {
        uint8_t tmp = digits[i];
        digits[i] = digits[len - 1 - i];
        digits[len - 1 - i] = tmp;
    }

    int rises = 0, falls = 0;
    /* Count adjacent digit comparisons in the normal order */
    for (int i = 0; i + 1 < len; ++i) {
        if (digits[i] < digits[i + 1])
            ++rises;
        else if (digits[i] > digits[i + 1])
            ++falls;
    }
    return rises > falls;
}

/* Main sequence generator */
seq_t a(idx_t n)
{
    /* Cache of previously computed terms */
    static seq_t *cache = NULL;  /* index 0 holds a(1) */
    static idx_t cache_len = 0;

    if (n == 0)   /* the offset is 1, so n=0 is below the sequence */
        return 0;

    /* Ensure the cache is large enough to hold n terms */
    if (cache_len < n) {
        size_t new_len = (size_t)n;
        if (cache == NULL) {
            cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        } else {
            cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        }
        if (cache == NULL)   /* allocation failure */
            abort();

        /* Compute missing terms */
        seq_t cur = (cache_len > 0) ? cache[cache_len - 1] + 1 : 1;
        for (; cache_len < n; ++cache_len) {
            while (!is_good(cur))
                ++cur;
            cache[cache_len] = cur;
            ++cur;
        }
    }

    return cache[n - 1];
}
