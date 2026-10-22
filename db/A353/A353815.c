/*
 * A353815  a(n) = 1 if sigma(n) is not a multiple of 3, otherwise 0.
 * https://oeis.org/A353815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 1 if sigma(n) is NOT a multiple of 3, otherwise 0
 * Range:     u8 n <= 107, u16 n <= 9580, u32 n <= 9580, u64 n <= 9580,
 *            u128 n <= 9580, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    seq_t s = sigma_of(n);
    return (s % 3 != 0) ? 1 : 0;
}
