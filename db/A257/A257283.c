/*
 * A257283  Numbers n not divisible by 3 such that n^2 written in base 3 has
 *          no digit > 1.
 * https://oeis.org/A257283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th natural number (n ≥ 1) that is not divisible by
 *            3 and whose square, written in base 3, contains only digits 0
 *            and 1
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 74, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t current = 1;           /* next number to test */

    if (n == 0) return 0;               /* undefined for n = 0 */

    /* Lazy allocation of cache */
    if (cache == NULL) {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n of them */
    while (cache_len < n) {
        if (current % 3 != 0) {
            seq_t sq = (seq_t)current * (seq_t)current;
            bool ok = true;
            while (sq > 0) {
                if (sq % 3 == 2) {      /* digit '2' found */
                    ok = false;
                    break;
                }
                sq /= 3;
            }
            if (ok) {
                if (cache_len == cache_cap) {     /* grow cache */
                    cache_cap <<= 1;
                    cache = (seq_t *)realloc(cache,
                                              cache_cap * sizeof(seq_t));
                }
                cache[cache_len++] = (seq_t)current;
            }
        }
        current++;
    }

    return cache[(size_t)n - 1];
}
