/*
 * A258057  First differences of the arithmetic derivative sequence A003415.
 * https://oeis.org/A258057
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = A003415(n+1) - A003415(n) (first differences of the
 *            arithmetic derivative)
 * Range:     i8 n <= 62, i16 n <= 6142, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   derivative
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/derivative.h"

seq_t a(idx_t n) {
    seq_t d_n    = derivative(n);
    seq_t d_np1  = derivative(n + 1);
    return d_np1 - d_n;
}
