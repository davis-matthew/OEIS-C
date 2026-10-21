/*
 * A282356  Eisenstein series E_26(q) (alternate convention E_13(q)),
 *          multiplied by 657931.
 * https://oeis.org/A282356
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -24 * sigma_{25}(n) for n >= 1, a(0) = 657931 where
 *            sigma_{k}(n) = sum_{d|n} d^k
 * Range:     i8 none, i16 none, i32 n <= 2, i64 n <= 5, i128 n <= 29,
 *            big any n
 * Helpers:   pow25, sigma25
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow25.h"
#include "../utils/sigma25.h"

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)657931;
    seq_t sigma = sigma25(n);
    return - (seq_t)24 * sigma;
}
