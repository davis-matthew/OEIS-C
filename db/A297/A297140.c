/*
 * A297140  Numbers having an up-first zigzag pattern in base 8; see Comments.
 * https://oeis.org/A297140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = n-th integer whose base‑8 digits form an up‑first zigzag
 *            pattern, i.e. the most significant digit is less than the next
 *            one and no two consecutive digits are equal
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   is_up_first_zigzag_base8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_up_first_zigzag_base8.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                          /* undefined for n < 1, return safe value */

    static seq_t *cache = NULL;
    static size_t cache_count = 0;
    static size_t cache_cap   = 0;

    if (cache_count < (size_t)n) {
        if (!cache) {
            cache_cap = 16;
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
            if (!cache)
                exit(1);
        }

        seq_t candidate = cache_count == 0 ? 10 : cache[cache_count - 1] + 1;
        while (cache_count < (size_t)n) {
            if (is_up_first_zigzag_base8(candidate)) {
                if (cache_count == cache_cap) {
                    size_t new_cap = cache_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp)
                        exit(1);
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_count++] = candidate;
            }
            ++candidate;
        }
    }

    return cache[(size_t)(n - 1)];
}
