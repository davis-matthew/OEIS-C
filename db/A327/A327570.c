/*
 * A327570  a(n) = n*phi(n)^2, phi = A000010.
 * https://oeis.org/A327570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = n * phi(n)^2 (phi = Euler's totient function)
 * Range:     u8 n <= 8, u16 n <= 40, u32 n <= 1626, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 64 terms through n = 9498 (2026-11-01)
 * Helpers:   totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/totient.h"

seq_t a(idx_t n) {
    seq_t ph = totient(n);
    seq_t sq = ph * ph;
    return (seq_t)n * sq;
}
