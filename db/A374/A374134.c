/*
 * A374134  a(n) = 1 if 2*phi(n) > n, otherwise 0, where phi is Euler's
 *          totient function phi, A000010.
 * https://oeis.org/A374134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if 2*phi(n) > n else 0
 * Range:     u8 n <= 100000, u16 n <= 100000, u32 n <= 100000,
 *            u64 n <= 100000, u128 n <= 100000, big any n
 * Validated: standard, pass, 141 terms through n = 8484 (2026-11-01)
 * Helpers:   totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/totient.h"

seq_t a(idx_t n)
{
    seq_t phi_val = totient(n);
    seq_t half = ((seq_t)n) >> 1;   // floor(n/2)
    return (phi_val > half) ? 1 : 0;
}
