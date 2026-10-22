/*
 * A382097  Sum of the legs of the unique primitive Pythagorean triple whose
 *          inradius is the n-th prime and whose short leg is an odd number.
 * https://oeis.org/A382097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*prime(n)^2 + 4*prime(n) + 1
 * Range:     u8 n <= 4, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n) {
    /* get the n-th prime (offset 1) */
    uint32_t p = get_prime(n);
    seq_t s = (seq_t)p;
    return 2*s*s + 4*s + 1;
}
