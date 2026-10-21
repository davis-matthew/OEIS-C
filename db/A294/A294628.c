/*
 * A294628  a(n) = 8*(sigma(n) - n + (1/2)).
 * https://oeis.org/A294628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*(sigma(n) - n + (1/2))
 * Range:     u8 n <= 23, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"

seq_t a(idx_t n) {
    seq_t sigma = sigma_of(n);
    seq_t sum_proper = sigma - (seq_t)n;
    return (seq_t)8 * sum_proper + (seq_t)4;
}
