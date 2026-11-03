/*
 * A241095  a(n) = (5^n - 1)^n.
 * https://oeis.org/A241095
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (5^n - 1)^n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Validated: partial, pass, 10 terms through n = 9 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)1;
    }
    seq_t pow5 = ipow((seq_t)5, (seq_t)n);
    pow5 -= (seq_t)1;
    return ipow(pow5, (seq_t)n);
}
