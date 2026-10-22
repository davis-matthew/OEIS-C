/*
 * A327570  a(n) = n*phi(n)^2, phi = A000010.
 * https://oeis.org/A327570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = n * phi(n)^2 (phi = Euler's totient function)
 * Range:     u8 n <= 8, u16 n <= 40, u32 n <= 1626, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/phi.h"

seq_t a(idx_t n) {
    seq_t ph = phi(n);
    seq_t sq = ph * ph;
    return (seq_t)n * sq;
}
