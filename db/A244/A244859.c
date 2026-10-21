/*
 * A244859  Least positive multiple of n which when written in base 10 is
 *          either a repunit or of the form 111...000.
 * https://oeis.org/A244859
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 16, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   factor_u64, mod_pow, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A244859: least positive multiple of n that is a
   repunit or a repunit followed by zeros. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/mod_pow.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Remove all factors 2 and 5 from n. */
    idx_t temp = n;
    unsigned a = 0, b = 0;
    while ((temp & 1ULL) == 0) { a++; temp >>= 1; }
    while (temp % 5ULL == 0)    { b++; temp  /= 5; }

    idx_t n_prime = temp;          /* part coprime to 10 */
    unsigned t = a > b ? a : b;    /* exponent for the trailing zeros */

    seq_t repunit = 1;             /* R_1 */

    if (n_prime != 1) {
        /* Find minimal length m such that n_prime | R_m.  This is the
           multiplicative order of 10 modulo 9*n_prime. */
        idx_t mod = 9 * n_prime;        /* modulus for the order */

        /* Factor mod to compute φ(mod). */
        idx_t primes_mod[64], exps_mod[64];
        size_t cnt_mod = 0;
        factor_u64(mod, primes_mod, exps_mod, &cnt_mod);

        idx_t phi = mod;
        for (size_t i = 0; i < cnt_mod; ++i) {
            phi -= phi / primes_mod[i];
        }

        /* Factor φ(mod) to reduce the order. */
        idx_t primes_phi[64], exps_phi[64];
        size_t cnt_phi = 0;
        factor_u64(phi, primes_phi, exps_phi, &cnt_phi);

        idx_t order = phi;
        for (size_t i = 0; i < cnt_phi; ++i) {
            idx_t p = primes_phi[i];
            while (order % p == 0) {
                idx_t cand = order / p;
                if (mod_pow(10, cand, (seq_t)mod) == 1)
                    order = cand;
                else
                    break;
            }
        }
        idx_t m = order;

        /* Build the repunit of length m. */
        repunit = 1;
        for (idx_t i = 1; i < m; ++i) {
            repunit = repunit * 10 + 1;
        }
    }

    /* Append the required zeros. */
    seq_t pow_t = pow10(t);
    return repunit * pow_t;
}
