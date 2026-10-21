/*
 * A248718  Numbers k such that k + (sum of digits of k) and k + (product of
 *          digits of k) contain the same distinct digits of k.
 * https://oeis.org/A248718
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth number k such that k + sum_of_digits(k) and k +
 *            product_of_digits(k) * contain exactly the same set of distinct
 *            digits as k, and k has no zero digit. * This is OEIS A248718
 *            (intersection of A247887 and A247888, with zeros removed)
 * Range:     u8 none, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   digit_counts, digit_product, digit_sum, seq_alloc_new_array,
 *            seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/digit_counts.h"
#include "../utils/digit_sum.h"
#include "../utils/digit_product.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;
    static seq_t next_candidate = 1;

    while (cache_len < (size_t)n) {
        seq_t k = next_candidate++;

        /* Count digits of k */
        unsigned char cntk[10];
        digit_counts(k, cntk);
        if (cntk[0]) /* Skip numbers with a zero digit */
            continue;

        /* Bitmask of distinct digits of k */
        unsigned bm_k = 0;
        for (int d = 0; d < 10; ++d)
            if (cntk[d]) bm_k |= (1u << d);

        seq_t sum = digit_sum(k);
        seq_t prod = digit_product(k);

        seq_t k_plus_sum = k + sum;
        seq_t k_plus_prod = k + prod;

        /* Distinct digits of k+sum */
        unsigned char cnts[10];
        digit_counts(k_plus_sum, cnts);
        unsigned bm_s = 0;
        for (int d = 0; d < 10; ++d)
            if (cnts[d]) bm_s |= (1u << d);

        /* Distinct digits of k+prod */
        unsigned char cntp[10];
        digit_counts(k_plus_prod, cntp);
        unsigned bm_p = 0;
        for (int d = 0; d < 10; ++d)
            if (cntp[d]) bm_p |= (1u << d);

        /* Check the definition */
        if (bm_s == bm_k && bm_p == bm_k) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *newp = seq_realloc(cache, cache_cap, new_cap);
                cache = newp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = k;
        }
    }
    return cache[n - 1];
}
