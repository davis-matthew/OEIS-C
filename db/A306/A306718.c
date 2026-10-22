/*
 * A306718  Numbers not of the form x^2+y^2+14*z^2.
 * https://oeis.org/A306718
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th positive integer not expressible as x^2 + y^2 +
 *            14*z^2 (x,y,z integers ≥0). This function generates the sequence
 *            by testing each integer sequentially, caching results for
 *            repeated calls
 * Range:     u8 n <= 37, u16 n <= 4226, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* static storage for caching results and the pre‑computed set of sums of two squares */
static bool *sum_two_squares = NULL;
static size_t sum_two_squares_size = 0;
static seq_t *cache = NULL;
static size_t cache_capacity = 0;
static size_t cache_len = 0;
static idx_t tested_upto = 0;

static void init_sum_two_squares(void)
{
    /* use a generous limit; the 10 000‑th term is well below 200 000 */
    const size_t LIMIT = 200000;
    sum_two_squares = (bool *)malloc((LIMIT + 1) * sizeof(bool));
    memset(sum_two_squares, 0, (LIMIT + 1) * sizeof(bool));
    for (idx_t x = 0; (idx_t)(x * x) <= LIMIT; ++x) {
        idx_t xx = (idx_t)(x * x);
        for (idx_t y = 0; (idx_t)(y * y) <= LIMIT - xx; ++y) {
            sum_two_squares[xx + y * y] = true;
        }
    }
    sum_two_squares_size = LIMIT + 1;
}

static void ensure_cache_capacity(size_t needed)
{
    if (cache_capacity == 0) {
        cache_capacity = 128;
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
    }
    while (cache_capacity < needed) {
        cache_capacity <<= 1;
        cache = (seq_t *)realloc(cache, cache_capacity * sizeof(seq_t));
    }
}

/* Return true if m = x^2 + y^2 + 14*z^2 for some x,y,z >= 0 */
static bool representable_by_form(idx_t m)
{
    for (idx_t z = 0;; ++z) {
        idx_t t = 14 * z * z;
        if (t > m) break;
        idx_t r = m - t;
        if (r < sum_two_squares_size && sum_two_squares[(size_t)r])
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (sum_two_squares == NULL)
        init_sum_two_squares();

    /* Expand cache until we have at least n elements */
    while (cache_len < n) {
        ++tested_upto;
        if (!representable_by_form(tested_upto)) {
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len++] = (seq_t)tested_upto;
        }
    }
    return cache[n - 1];
}
