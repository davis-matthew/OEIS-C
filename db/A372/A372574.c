/*
 * A372574  a(n) = 1 if the squarefree part of n is congruent to 1 or 5 modulo
 *          6, otherwise 0.
 * https://oeis.org/A372574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = 1 iff the squarefree part of n ≡ 1 or 5 (mod 6), else 0
 * Range:     u8 n <= 8780, u16 n <= 8780, u32 n <= 8780, u64 n <= 8780,
 *            u128 n <= 8780, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t temp = n;            /* local copy to modify during factorisation */
    uint64_t res_mod = 1 % 6;      /* holds the squarefree part modulo 6 */

    /* trial division up to √temp */
    for (uint64_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            if (exp & 1) {                 /* odd exponent → include p */
                res_mod = (res_mod * (p % 6)) % 6;
            }
        }
    }

    /* remaining prime factor (if any) has exponent 1 (odd) */
    if (temp > 1) {
        res_mod = (res_mod * (temp % 6)) % 6;
    }

    return (res_mod == 1 || res_mod == 5) ? (seq_t)1 : (seq_t)0;
}
