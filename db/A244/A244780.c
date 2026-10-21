/*
 * A244780  Positive numbers primitively represented by the binary quadratic
 *          form (1, 1, 3).
 * https://oeis.org/A244780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) are the positive integers primitively represented by the
 *            binary quadratic form (1, 1, 3)
 * Range:     u8 n <= 6, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   floor_sqrt, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/floor_sqrt.h"
#include "../utils/gcd.h"


/* Check whether a number m has a primitive representation as x² + x y + 3 y². */
static bool is_primitive_rep(seq_t m)
{
    seq_t four_m = (seq_t)4 * m;

    for (idx_t y = 0; ; ++y) {
        seq_t y_seq = (seq_t)y;
        seq_t y_sq  = y_seq * y_seq;
        seq_t lhs   = (seq_t)11 * y_sq;
        if (lhs > four_m) break;

        seq_t rhs = four_m - lhs;
        seq_t t   = floor_sqrt(rhs);
        if (t * t != rhs) continue;

        /* t and y must have the same parity to make x integer. */
        if (((t ^ y_seq) & 1) != 0) continue;

        seq_t x_abs = (t > y_seq) ? ((t - y_seq) >> 1) : ((y_seq - t) >> 1);
        if (gcd(x_abs, y_seq) == (seq_t)1) return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    static seq_t *cache       = NULL;
    static size_t cache_size  = 0;   /* number of terms stored */
    static size_t cache_cap   = 0;   /* allocated capacity */
    static seq_t last_checked = 0;   /* last integer examined */

    if (n == 0) return 0;            /* undefined for n = 0, but safe */

    if ((size_t)n <= cache_size) return cache[n - 1];

    if (n > cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
        while (new_cap < (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    while (cache_size < (size_t)n) {
        ++last_checked;
        if (is_primitive_rep(last_checked)) {
            cache[cache_size++] = last_checked;
        }
    }

    return cache[n - 1];
}
