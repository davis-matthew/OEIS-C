/*
 * A343680  Niven (or Harshad) numbers which when divided by sum of their
 *          digits, give a quotient which is a Zuckerman number.
 * https://oeis.org/A343680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   digit_product_all_nonzero, seq_alloc_new_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A343680: Niven (Harshad) numbers whose quotient
 * (number divided by the sum of its digits) is a Zuckerman number.
 * The implementation searches the positive integers in order,
 * keeping a memoised list of terms.  Complexity is O(n) with a tiny
 * constant factor, suitable for n up to at least 10 000. */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_realloc.h"
#include "../utils/digit_product_all_nonzero.h"

/* sum of decimal digits of x */
static seq_t digit_sum(seq_t x) {
    seq_t s = 0;
    while (x > 0) {
        s += x % 10;
        x /= 10;
    }
    return s;
}

/* true iff v is a Niven number whose quotient is a Zuckerman number */
static bool good_number(seq_t v) {
    seq_t original = v;
    seq_t s = digit_sum(v);
    if (s == 0)
        return false;
    if (original % s != 0)
        return false;
    seq_t q = original / s;
    seq_t prod = digit_product_all_nonzero(q);
    if (prod == 0)               /* q contains a zero digit */
        return false;
    return (q % prod == 0);
}

seq_t a(idx_t n) {
    static seq_t *cache = NULL;      /* memoised list of terms */
    static size_t cache_cap = 0;     /* capacity of cache array */
    static size_t cache_len = 0;     /* number of terms stored */
    static seq_t next_num = 1;       /* next integer to test */

    /* initialise cache the first time */
    if (cache == NULL) {
        cache_cap = 64;
        cache = seq_alloc_new_array(cache_cap);
    }

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        if (good_number(next_num)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap * 2;
                cache = seq_realloc(cache, cache_cap, new_cap);
                cache_cap = new_cap;
            }
            cache[cache_len++] = next_num;
        }
        ++next_num;
    }

    return cache[(size_t)n - 1];
}
