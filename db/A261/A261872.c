/*
 * A261872  a(n) = phi(n) mod 5, where phi is the Euler totient function.
 * https://oeis.org/A261872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = phi(n) mod 5
 * Range:     u8 n <= 367, u16 n <= 9821, u32 n <= 9821, u64 n <= 9821,
 *            u128 n <= 9821, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"
#include <stdbool.h>

seq_t a(idx_t n)
{
    return phi(n) % (seq_t)5;
}
