/*
 * A327812  Number of irreducible factors in the factorization of the n-th
 *          cyclotomic polynomial over GF(3) (counted with multiplicity).
 * https://oeis.org/A327812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = phi(n)/ord(3,s) where n = 3^e * s with gcd(3,s)=1
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   calc_phi, ord3_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#include "../utils/calc_phi.h"  /* provides seq_t calc_phi(seq_t) */
#include "../utils/ord3_mod.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined outside offset, but safe default */

    /* separate the factor of 3 */
    idx_t tmp = n;
    while (tmp % 3 == 0) tmp /= 3;
    uint64_t s = (uint64_t)tmp;  /* s is not divisible by 3 */

    seq_t phi_n = calc_phi((seq_t)n);
    seq_t ord = ord3_mod(s);
    return phi_n / ord;
}
