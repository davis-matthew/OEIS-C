/*
 * A344864  a(n) = mu(d(n)).
 * https://oeis.org/A344864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = mu(d(n))
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, mu_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/divisor_count_u64.h"
#include "../utils/mu_of.h"

seq_t a(idx_t n) {
    uint64_t tau = divisor_count_u64(n);
    return (seq_t)mu_of(tau);
}
