/*
 * A385158  Numbers k such that the sum of the digits of k is a number that
 *          appears as a substring of k, and every nonzero digit of k appears
 *          in that sum.
 * https://oeis.org/A385158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 51 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: generate numbers k in ascending order and test
   the property: digit sum of k appears as a substring of k and
   every nonzero digit of k appears in that sum.  Store found
   numbers in a cache and return the n‑th term. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

static bool has_sum_substring_and_digits(seq_t k)
{
    seq_t temp = k;
    seq_t sum = 0;
    unsigned mask_k = 0;          /* bits for nonzero digits of k */
    /* compute digit sum and mask of nonzero digits of k */
    while (temp > 0) {
        unsigned d = (unsigned)(temp % 10);
        sum += d;
        if (d != 0)
            mask_k |= 1U << (d - 1);
        temp /= 10;
    }

    /* mask of digits present in the sum */
    unsigned mask_sum = 0;
    seq_t t2 = sum;
    while (t2 > 0) {
        unsigned d = (unsigned)(t2 % 10);
        mask_sum |= 1U << (d - 1);
        t2 /= 10;
    }
    /* every nonzero digit of k must appear in the sum */
    if ((mask_k & mask_sum) != mask_k)
        return false;

    /* build decimal representations of k and sum */
    char buf_k[128], buf_sum[32];
    int len_k = 0, len_sum = 0;

    seq_t t3 = k;
    if (t3 == 0)
        buf_k[len_k++] = '0';
    while (t3 > 0) {
        buf_k[len_k++] = '0' + (unsigned)(t3 % 10);
        t3 /= 10;
    }
    for (int i = 0; i < len_k / 2; ++i) {
        char tmp = buf_k[i];
        buf_k[i] = buf_k[len_k - 1 - i];
        buf_k[len_k - 1 - i] = tmp;
    }

    seq_t t4 = sum;
    if (t4 == 0)
        buf_sum[len_sum++] = '0';
    while (t4 > 0) {
        buf_sum[len_sum++] = '0' + (unsigned)(t4 % 10);
        t4 /= 10;
    }
    for (int i = 0; i < len_sum / 2; ++i) {
        char tmp = buf_sum[i];
        buf_sum[i] = buf_sum[len_sum - 1 - i];
        buf_sum[len_sum - 1 - i] = tmp;
    }

    /* substring test */
    for (int i = 0; i + len_sum <= len_k; ++i) {
        int j;
        for (j = 0; j < len_sum; ++j) {
            if (buf_k[i + j] != buf_sum[j])
                break;
        }
        if (j == len_sum)
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)      /* undefined for offset 0 */
        return 0;

    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t last_k = 0;   /* last k checked or stored */

    if (n <= cache_len)
        return cache[n - 1];

    if (cache == NULL) {
        cache_cap = 64;
        cache = mem_new(seq_t, cache_cap);
    }

    seq_t k = last_k + (seq_t)1;
    while (cache_len < n) {
        if (has_sum_substring_and_digits(k)) {
            if (cache_len == cache_cap) {
                cache_cap = cache_cap * 2;
                cache = mem_resize(seq_t, cache, cache_cap);
            }
            cache[cache_len++] = k;
        }
        ++k;
    }
    last_k = k - (seq_t)1;
    return cache[n - 1];
}
