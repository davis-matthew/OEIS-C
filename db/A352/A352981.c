/*
 * A352981  a(n) = Sum_{k=0..floor(n/2)} k^n.
 * https://oeis.org/A352981
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} k^n
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 31,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 425 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    idx_t m = n >> 1;          /* floor(n/2) */
    seq_t sum = 0;

    for (idx_t k = 0; k <= m; ++k) {
        seq_t term;
        if (k == 0) {
            /* 0^0 is treated as 1; otherwise 0^n = 0 */
            term = (n == 0) ? (seq_t)1 : (seq_t)0;
        } else {
            term = ipow((seq_t)k, (seq_t)n);
        }
        sum += term;
    }
    return sum;
}
