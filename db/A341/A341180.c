/*
 * A341180  Numbers that when divided by the sum of their digits leave 13 as
 *          remainder.
 * https://oeis.org/A341180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 1938, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 64 terms through n = 9639 (2026-11-03)
 * Helpers:   digit_sum, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the n‑th positive integer m such that m % digit_sum(m) == 13 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    if (n == 0) return 0; /* not used – offset is 1 */

    while (cache_len < n) {
        seq_t cur = (cache_len == 0) ? (seq_t)1 : cache[cache_len - 1] + (seq_t)1;
        for (;;) {
            seq_t ds = digit_sum(cur);
            if (ds > (seq_t)13 && cur % ds == (seq_t)13) {
                if (cache_len == cache_cap) {
                    cache_cap = cache_cap ? cache_cap * 2 : 16;
                    seq_t *tmp = mem_resize(seq_t, cache, cache_cap);
                    if (!tmp) abort();
                    cache = tmp;
                }
                cache[cache_len++] = cur;
                break;
            }
            ++cur;
        }
    }
    return cache[n - 1];
}
