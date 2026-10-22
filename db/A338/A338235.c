/*
 * A338235  Numbers k such that k + the sum of the 4th powers of the decimal
 *          digits of k is a square.
 * https://oeis.org/A338235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 10, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: return the n‑th integer k such that
 * k + the sum of the 4th powers of the decimal digits of k is a perfect square. */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/floor_sqrt.h"

static const int pow4_int[10] = {
    0,     /* 0^4 */
    1,     /* 1^4 */
    16,    /* 2^4 */
    81,    /* 3^4 */
    256,   /* 4^4 */
    625,   /* 5^4 */
    1296,  /* 6^4 */
    2401,  /* 7^4 */
    4096,  /* 8^4 */
    6561   /* 9^4 */
};

/* cache for previously found sequence terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static seq_t last_k = 0;

/* Ensure the cache can hold at least 'need' elements. */
static void ensure_capacity(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Test whether x is a perfect square using floor_sqrt. */
static int is_square(seq_t x)
{
    seq_t r = floor_sqrt(x);
    return r * r == x;
}

/* Sum of fourth powers of the decimal digits of k. */
static seq_t digit_sum4(seq_t k)
{
    seq_t sum = 0;
    while (k > 0) {
        int d = (int)(k % 10);
        sum += (seq_t)pow4_int[d];
        k /= 10;
    }
    return sum;
}

/* Main sequence routine */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* defined only for n >= 1 */

    /* extend cache until it contains at least n terms */
    while (cache_len < (size_t)n) {
        seq_t k = last_k + 1;   /* next candidate */
        for (;;) {
            seq_t s = digit_sum4(k);
            seq_t v = k + s;
            if (is_square(v)) {
                ensure_capacity(cache_len + 1);
                cache[cache_len++] = k;
                last_k = k;
                break;
            }
            k++;
        }
    }

    return cache[(size_t)n - 1];
}
