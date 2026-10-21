/*
 * A291539  a(n) = PrimePi(n^3) - PrimePi(n) * PrimePi(n^2), where PrimePi =
 *          A000720.
 * https://oeis.org/A291539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = PrimePi(n^3) - PrimePi(n) * PrimePi(n^2)
 * Range:     u8 n <= 17, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/prime_pi.h"

seq_t a(idx_t n) {
    idx_t n2 = n * n;
    idx_t n3 = n2 * n;

    seq_t pi_n  = prime_pi(n);
    seq_t pi_n2 = prime_pi(n2);
    seq_t pi_n3 = prime_pi(n3);

    return pi_n3 - pi_n * pi_n2;
}
