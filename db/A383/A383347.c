/*
 * A383347  Numbers that have the same set of digits as the sum of the cubes
 *          of their digits.
 * https://oeis.org/A383347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth integer x such that the set of its decimal digits
 *            equals the set of the decimal digits of the sum of the cubes of
 *            its digits
 * Range:     u8 n <= 2, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   append_candidate
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/append_candidate.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap = 0;

    if (n == 0) return 0;  /* undefined, but safe */

    while (cache_size < (size_t)n) {
        /* start searching from the first candidate after the last found term */
        seq_t candidate = (cache_size == 0) ? 0 : cache[cache_size - 1] + 1;

        for (;;) {
            /* compute sum of cubes of digits of candidate */
            seq_t sum = 0;
            seq_t tmp = candidate;
            if (tmp == 0) {
                sum = 0;
            } else {
                while (tmp > 0) {
                    unsigned d = (unsigned)(tmp % 10);
                    sum += (seq_t)d * d * d;
                    tmp /= 10;
                }
            }

            /* compute digit masks of candidate and of sum */
            unsigned mask1 = 0, mask2 = 0;
            tmp = candidate;
            if (tmp == 0) {
                mask1 = 1u << 0;
            } else {
                while (tmp > 0) {
                    unsigned d = (unsigned)(tmp % 10);
                    mask1 |= 1u << d;
                    tmp /= 10;
                }
            }

            tmp = sum;
            if (tmp == 0) {
                mask2 = 1u << 0;
            } else {
                while (tmp > 0) {
                    unsigned d = (unsigned)(tmp % 10);
                    mask2 |= 1u << d;
                    tmp /= 10;
                }
            }

            /* if the digit sets are equal, store the term */
            if (mask1 == mask2) {
                append_candidate(&cache, &cache_size, &cache_cap, candidate);
                break;  /* go on to find the next term */
            }
            /* otherwise try the next integer */
            candidate++;
        }
    }

    return cache[(size_t)(n - 1)];
}
