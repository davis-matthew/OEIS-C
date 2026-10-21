/*
 * A273363  Numbers m such that the decimal number concat(8,m) is a square.
 * https://oeis.org/A273363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 5406, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_seq_array, digit_len, free_seq_array, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS A273363: Numbers m such that concat(8,m) is a square */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/digit_len.h"
#include "../utils/pow10.h"
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n < 1 */

    /* static cache for memoisation of generated terms */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t last_k = 0;   /* last processed k (square root) */

    while (cache_len < (size_t)n) {
        /* generate next candidate by increasing k */
        ++last_k;
        seq_t ksq = (seq_t)last_k * (seq_t)last_k;

        int d = digit_len(ksq);               /* number of digits of k^2 */
        if (d < 2) continue;                  /* need at least 2 digits: leading 8 and a non‑zero m */

        seq_t first = ksq / pow10((idx_t)(d - 1));   /* most significant digit of k^2 */
        if (first != 8) continue;               /* not starting with 8 */

        seq_t base   = 8 * pow10((idx_t)(d - 1));  /* 8 * 10^(d−1)   */
        seq_t m      = ksq - base;               /* candidate m */

        seq_t min_m  = pow10((idx_t)(d - 2));    /* lower bound to avoid leading zero */
        if (m < min_m) continue;

        /* grow cache if necessary */
        if (cache_len == cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 4 : cache_cap * 2;
            seq_t *tmp = alloc_seq_array(new_cap);
            for (size_t i = 0; i < cache_len; ++i) tmp[i] = cache[i];
            if (cache) free_seq_array(cache);
            cache = tmp;
            cache_cap = new_cap;
        }

        cache[cache_len++] = m;
    }

    return cache[n - 1];
}
