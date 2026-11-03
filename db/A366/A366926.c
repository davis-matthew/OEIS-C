/*
 * A366926  Odd numbers m that can be written j * k, j >= k > 1, with
 *          floor(log_2(m)) = floor(log_2(j)) + floor(log_2(k)).
 * https://oeis.org/A366926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   odd m that satisfy j*k with j>=k>1 and * floor(log2(m)) =
 *            floor(log2(j)) + floor(log2(k))
 * Range:     u8 n <= 46, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9699 (2026-11-03)
 * Helpers:   bit_length, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function searches odd numbers incrementally and memoises the
 * ones that meet the condition.  It uses the provided bitlen_seq_t
 * utility to compute floor(log2) via the bit length. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/bit_length.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t candidate = (seq_t)3;          /* odd numbers start at 3 */

    if (n == 0) return 0;                      /* safety guard */

    while (cache_len < (size_t)n) {
        seq_t m = candidate;
        unsigned bitlen_m = bit_length(m);
        bool good = false;

        /* trial division by odd divisors up to sqrt(m) */
        seq_t d = (seq_t)3;
        while (d <= m / d) {
            if (m % d == (seq_t)0) {
                seq_t k = d;
                seq_t j = m / d;               /* j >= k > 1 */
                if (bit_length(j) + bit_length(k) - 1 == bitlen_m) {
                    good = true;
                    break;
                }
            }
            d += (seq_t)2;                     /* next odd divisor */
        }

        if (good) {
            if (cache_len == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = mem_resize(seq_t, cache, cache_cap);
                cache = tmp;
            }
            cache[cache_len++] = m;
        }
        candidate += (seq_t)2;                 /* next odd integer */
    }

    return cache[(size_t)(n - 1)];
}
