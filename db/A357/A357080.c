/*
 * A357080  Numbers k such that the sum of the digits of k multiplied by the
 *          sum of the digits of k^2 equals k.
 * https://oeis.org/A357080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A357080: Numbers k such that sum_digits(k) * sum_digits(k^2) == k
   The sequence is finite; all such k have at most 4 decimal digits.
   This implementation scans all integers from 0 to 9999 once,
   collects the valid k's, and returns the nth element.
   A(1) = 0, A(2) = 1, A(3) = 80, ... */

#include "../utils/digit_sum.h"
#include <stddef.h>

seq_t a(idx_t n) {
    /* static storage for all found numbers; max possible is < 2600 */
    static seq_t matches[2600];
    static size_t matches_len = 0;
    static bool initialized = false;

    if (!initialized) {
        for (idx_t k = 0; k <= 9999; ++k) {
            seq_t k_t = (seq_t)k;
            seq_t square = k_t * k_t;
            seq_t sum_k = digit_sum(k_t);
            seq_t sum_sq = digit_sum(square);
            if (sum_k * sum_sq == k_t) {
                matches[matches_len++] = k_t;
            }
        }
        initialized = true;
    }

    if (n == 0 || (idx_t)n > (idx_t)matches_len) return 0;
    return matches[(size_t)(n - 1)];
}
