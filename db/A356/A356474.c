/*
 * A356474  a(n) = phi(rad(prime(n)-1)), where phi = A000010 and rad =
 *          A007947.
 * https://oeis.org/A356474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = phi(rad(prime(n)-1))
 * Range:     u8 n <= 55, u16 n <= 6846, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi, get_prime, rad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/rad.h"
#include "../utils/calc_phi.h"

seq_t a(idx_t n) {
    if (n < 1) return 0;
    uint32_t p = get_prime(n);
    seq_t r = rad((idx_t)p - 1);
    return calc_phi(r);
}
