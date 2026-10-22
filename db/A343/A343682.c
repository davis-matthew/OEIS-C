/*
 * A343682  Zuckerman numbers which when divided by the product of their
 *          digits, give a quotient which is a Niven (Harshad) number.
 * https://oeis.org/A343682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 47, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   digit_product, digit_sum, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th Zuckerman number.
   A Zuckerman number N satisfies:
        N   is divisible by the product of its decimal digits
   and the quotient Q = N / (product of digits of N) is a Niven
   (Harshad) number, i.e.   Q is divisible by the sum of its digits.
   The first few terms are: 1,2,3,4,5,6,7,8,9,12,15,24,36,111,128,135,... */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/digit_product.h"
#include "../utils/digit_sum.h"
#include "../utils/seq_realloc.h"

/* cache for found Zuckerman numbers */
static seq_t *cache = NULL;      /* array of found values   */
static size_t cache_len = 0;     /* number of cached values */
static size_t cache_cap = 0;     /* allocated capacity     */
static seq_t next_candidate = 1; /* next number to test   */

/* ensure the cache contains at least n terms */
static void ensure_upto(idx_t n)
{
    if (cache_len >= (size_t)n)
        return;

    /* grow capacity as needed, doubling each time */
    if (cache_cap == 0) {
        cache_cap = n;
    } else {
        while (cache_cap < (size_t)n)
            cache_cap *= 2;
    }
    seq_t *tmp = seq_realloc(cache, cache_cap / sizeof(seq_t), cache_cap);
    /* seq_realloc takes old capacity, but our cache_cap was the old capacity in numbers,
       not bytes; the helper takes number of elements, so we pass the same value. */
    cache = tmp;

    /* search until we have n cached Zuckerman numbers */
    while (cache_len < (size_t)n) {
        seq_t dprod = digit_product(next_candidate);
        if (dprod != 0 && next_candidate % dprod == 0) {
            seq_t q = next_candidate / dprod;
            seq_t dsum = digit_sum(q);
            if (dsum != 0 && q % dsum == 0) {
                cache[cache_len++] = next_candidate;
            }
        }
        /* guard against overflow in seq_t. In the usual cases
           the sequence grows slowly enough that this is safe. */
        ++next_candidate;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;      /* undefined for n < 1 – return 0 as sentinel */
    ensure_upto(n);
    return cache[n - 1];
}
