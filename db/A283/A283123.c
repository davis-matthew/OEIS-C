/*
 * A283123  a(n) = sigma(9*n).
 * https://oeis.org/A283123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(9*n)
 * Range:     u8 n <= 11, u16 n <= 1959, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"

seq_t a(idx_t n) {
    seq_t m = ((seq_t)n) * (seq_t)9;
    return sigma_of((idx_t)m);
}
