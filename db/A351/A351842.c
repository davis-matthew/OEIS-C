/*
 * A351842  Numbers whose sum of digits and number of proper divisors are
 *          equal.
 * https://oeis.org/A351842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   digit_sum, divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose digit sum equals the number of proper divisors.
   a(n) = n-th integer such that digit_sum(n) == divisor_count(n) - 1.
   The function builds a cache lazily and searches integers sequentially.
*/

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/digit_sum.h"
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap = 0;
    static uint64_t next = 1;           /* next integer to test (starts at 2) */

    while (cache_size < (size_t)n) {
        uint64_t cand = ++next;         /* candidate integer */

        seq_t dsum = digit_sum((seq_t)cand);
        uint64_t dcnt_u64 = divisor_count_u64(cand) - 1;
        seq_t dcnt = (seq_t)dcnt_u64;

        if (dsum == dcnt) {
            if (cache_size + 1 > cache_cap) {
                size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                    abort();          /* memory allocation failed */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)cand;
        }
    }
    return cache[(size_t)n - 1];
}
