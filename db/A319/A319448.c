/*
 * A319448  Moebius function mu(n) defined for the Eisenstein integers.
 * https://oeis.org/A319448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = 0 if 3 | n or n has a squared prime factor, otherwise
 *            a(n) = ∏_{p | n} (3 - 2*(p mod 3)). For n square‑free and n not
 *            divisible by 3 this product is ±1
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   eig_mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/eig_mobius.h"

seq_t a(idx_t n)
{
    /* Cast idx_t to uint64_t for safety; n is always ≥1. */
    uint64_t m = (uint64_t)n;
    return eig_mobius(m);
}
