/*
 * A372677  a(n) = phi(13 * n)/12.
 * https://oeis.org/A372677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = phi(13*n)/12
 * Range:     u8 n <= 76, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/calc_phi.h"

/* Computes a(n) = phi(13*n)/12 as defined in OEIS A372677.
 * The function uses the provided calc_phi to compute φ(m)
 * and handles the special case where 13 divides n. */
seq_t a(idx_t n) {
    /* Count the exponent of 13 in n */
    idx_t tmp = n;
    idx_t e = 0;
    while (tmp % 13 == 0) {
        tmp /= 13;
        e++;
    }

    /* m = n / 13^e  (now coprime with 13) */
    seq_t m = (seq_t)tmp;

    /* φ(m) via helper */
    seq_t phi_m = calc_phi(m);

    /* 13^e as seq_t */
    seq_t pow13 = 1;
    for (idx_t i = 0; i < e; ++i) {
        pow13 *= 13;
    }

    /* a(n) = 13^e * φ(m)      (for e==0 this reduces to φ(n)) */
    return pow13 * phi_m;
}
