/*
 * A379676  For n >= 0, a(n) is the least k >= 2 such that (n + 1)*(2*k + n) /
 *          2 is a triangular number (A000217).
 * https://oeis.org/A379676
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k >= 2 such that (n+1)*(2*k+n)/2 is a triangular
 *            number
 * Range:     u8 n <= 0, u16 n <= 30, u32 n <= 8156, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    /* g = n + 1 (as seq_t to avoid intermediate overflow) */
    seq_t g = (seq_t)n + 1;
    for (idx_t k = 2; ; ++k) {
        /* Compute S = (n+1)*(2*k + n)/2 using only integer arithmetic */
        seq_t two_k_n = (seq_t)2 * k + (seq_t)n;   /* 2*k + n as seq_t */
        seq_t S = g * two_k_n / 2;                 /* (n+1)*(2*k+n)/2 */

        /* Check if S is triangular: 8*S + 1 must be a perfect square   */
        seq_t t2 = 8 * S + 1;                      /* 8*S + 1 */
        seq_t m  = floor_sqrt(t2);                 /* floor(sqrt(8*S+1)) */
        if (m * m == t2)                          /* perfect square?   */
            return (seq_t)k;                      /* minimal k found   */
    }
}
