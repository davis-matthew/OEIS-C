/*
 * A253626  Expansion of psi(q^2) * f(q, q^2)^2 / f(q, q^5) in powers of q
 *          where psi(), f() are Ramanujan theta functions.
 * https://oeis.org/A253626
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(0) = 1 For n > 0, let n = ∏p^e be the prime factorisation.
 *            a(n) = ∏ f(p,e) where f(2,e) = 3 (e>0) f(3,e) = 1 f(p,e) = e+1
 *            if p%6 == 1 f(p,e) = (1 + (-1)^e)/2 if p%6 == 5 (Thus for a
 *            prime p≡5 (mod 6), f(p,odd)=0, f(p,even)=1.)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mult_val
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/mult_val.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t result = (seq_t)1;
    uint64_t tmp = (uint64_t)n;
    for (uint64_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            result *= mult_val(p, e);
            if (result == (seq_t)0) return (seq_t)0; /* early exit */
        }
    }
    if (tmp > 1) {
        /* remaining prime factor */
        result *= mult_val(tmp, 1);
    }
    return result;
}
