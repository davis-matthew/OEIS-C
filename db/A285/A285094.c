/*
 * A285094  Corresponding values of geometric means of digits of numbers from
 *          A061430.
 * https://oeis.org/A285094
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = k-th root of the digit product of A061430(n+1), k =
 *            number of digits (found by search)
 * Range:     u8 n <= 82, u16 n <= 3000, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Validated: standard, pass, 103 terms through n = 2895 (2026-11-08)
 * Helpers:   digit_product, ipow, mem, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/digit_product.h"
#include "../utils/ipow.h"
#include "../utils/num_digits.h"
#include "../utils/mem.h"

static seq_t A061430_kth_root(seq_t v, unsigned int k)
{
    if (v == 0) return 0;
    seq_t low = 1;
    seq_t high = 1;
    while (true) {
        seq_t p = ipow(high, (seq_t)k);
        if (p > v || p == 0)
            break;
        if (p == v) return high;
        high <<= 1;
    }
    while (low + 1 < high) {
        seq_t mid = (low + high) >> 1;
        seq_t p = ipow(mid, (seq_t)k);
        if (p <= v)
            low = mid;
        else
            high = mid;
    }
    return low;
}

static bool A061430_good_number(seq_t num)
{
    seq_t prod = digit_product(num);
    if (prod == 0)
        return true;
    unsigned int k = (unsigned int)num_digits(num);
    seq_t m = A061430_kth_root(prod, k);
    return ipow(m, (seq_t)k) == prod;
}

static seq_t A061430_a(idx_t n)
{
    if (n < 1)
        return 0;
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t next = 0;

    while (cache_len < (size_t)n) {
        if (A061430_good_number(next)) {
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp)
                    return 0;
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = next;
        }
        ++next;
    }
    return cache[(size_t)n - 1];
}

seq_t a(idx_t n)
{
    seq_t num = A061430_a(n + 1);
    seq_t prod = digit_product(num);
    if (prod == 0)
        return 0;
    unsigned int k = (unsigned int)num_digits(num);
    return A061430_kth_root(prod, k);
}
