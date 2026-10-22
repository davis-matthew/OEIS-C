/*
 * A340164  Rotationally ambigrammatic square numbers with no trailing zeros.
 * https://oeis.org/A340164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = square of the nth integer whose square is rotationally
 *            ambigrammatic (digits 0, 1, 6, 8, 9) with no trailing zero; zero
 *            is allowed as the first term
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   alloc_cache, valid_square_uint128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/alloc_cache.h"
#include "../utils/valid_square_uint128.h"

static const uint8_t ALLOWED_DIGITS[5] = {0, 1, 6, 8, 9};

/* Static storage for memoised terms. */
static seq_t *cache        = NULL; /* array of terms */
static idx_t cache_len     = 0;    /* number of terms stored */
static idx_t cache_cap     = 0;    /* allocated capacity            */
static idx_t next_root     = 0;    /* next integer to test as root  */

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)          /* safety: only defined for n >= 1 */
        return 0;

    if (cache_cap == 0) { /* initial allocation */
        cache_cap = 64;
        cache     = alloc_cache(cache_cap);
    }

    /* Generate terms until we have the requested index. */
    while (cache_len < n) {
        /* Compute the square of the next root using 128‑bit arithmetic
           to avoid overflow when seq_t is small. */
        __uint128_t root128 = (__uint128_t)next_root;
        __uint128_t sq128   = root128 * root128;
        ++next_root;

        if (!valid_square_uint128(sq128))
            continue;          /* not a valid term */

        /* Grow cache if needed. */
        if (cache_len == cache_cap) {
            idx_t new_cap = cache_cap * 2;
            seq_t *tmp = alloc_cache(new_cap);
            for (idx_t i = 0; i < cache_len; ++i)
                tmp[i] = cache[i];
            cache = tmp;
            cache_cap = new_cap;
        }

        cache[cache_len++] = (seq_t)sq128;
    }

    return cache[n - 1];
}
