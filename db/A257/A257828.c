/*
 * A257828  Positive integers whose square is the sum of 97 consecutive
 *          squares.
 * https://oeis.org/A257828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 125619266*a(n-3) - a(n-6) Initial terms: a(1)=679
 *            a(2)=1545404 a(3)=3675742735 a(4)=81619738879
 *            a(5)=194132514608060 a(6)=461744104375531831
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_realloc.h"

static const seq_t init_vals[6] = {
    (seq_t)679,
    (seq_t)1545404,
    (seq_t)3675742735,
    (seq_t)81619738879,
    (seq_t)194132514608060,
    (seq_t)461744104375531831
};

seq_t a(idx_t n)
{
    /* Handle the first six terms directly */
    if (n <= 6) {
        return init_vals[n - 1];
    }

    /* Static cache for computed terms */
    static seq_t *cache = NULL;
    static size_t cached_len = 0;   /* number of terms stored */

    /* Ensure cache holds at least six terms */
    if (cached_len < 6) {
        seq_t *tmp = seq_realloc(cache, cached_len, 6);
        cache = tmp;
        for (size_t i = cached_len; i < 6; ++i) {
            cache[i] = init_vals[i];
        }
        cached_len = 6;
    }

    /* Grow cache up to requested n */
    if (cached_len < n) {
        seq_t *tmp = seq_realloc(cache, cached_len, (size_t)n);
        cache = tmp;
        for (size_t i = cached_len; i < (size_t)n; ++i) {
            seq_t term = ((seq_t)125619266) * cache[i - 3];
            term -= cache[i - 6];
            cache[i] = term;
        }
        cached_len = (size_t)n;
    }

    return cache[n - 1];
}
