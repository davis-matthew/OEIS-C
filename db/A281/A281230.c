/*
 * A281230  Period of the discrete Arnold cat map on an n X n array.
 * https://oeis.org/A281230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001175(n)/2 for n ≥ 3, a(1)=1, a(2)=3 (A001175(n) is the
 *            Pisano period of the Fibonacci sequence modulo n.)
 * Range:     u8 n <= 49, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pisano_period
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pisano_period.h"

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    seq_t period = pisano_period(n);
    /* For n ≥ 3 the period is even, so the division is exact. */
    return period / (seq_t)2;
}
