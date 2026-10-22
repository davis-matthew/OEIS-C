/*
 * A327815  Number of irreducible factors in the factorization of the n-th
 *          cyclotomic polynomial over GF(7) (counted with multiplicity).
 * https://oeis.org/A327815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = phi(n) / ord(7, s), where n = 7^e * s , gcd(7,s)=1
 * Range:     u8 n <= 341, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi, factor_u64, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/calc_phi.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                       /* undefined; guard prototype */

    /* Separate the 7‑part of n */
    idx_t temp = n;
    while (temp % 7 == 0) temp /= 7;
    idx_t s = temp;                              /* s is coprime with 7 */

    /* φ(n) and φ(s) */
    seq_t phi_n = calc_phi((seq_t)n);
    seq_t phi_s = calc_phi((seq_t)s);

    /* compute multiplicative order of 7 modulo s */
    seq_t ord_seq = phi_s;                      /* initial order = φ(s) */

    if (s != 1) {
        uint64_t ord64 = (uint64_t)ord_seq;     /* φ(s) fits in 64‑bit for typical use */

        /* factor the order to speed up reduction */
        uint64_t primes[64], exps[64];
        size_t cnt;
        factor_u64(ord64, primes, exps, &cnt);

        for (size_t i = 0; i < cnt; ++i) {
            uint64_t p = primes[i];
            while (ord64 % p == 0) {
                seq_t r = mod_pow((seq_t)7, ord64 / p, (seq_t)s);
                if (r == (seq_t)1) {
                    ord64 /= p;
                } else {
                    break;
                }
            }
        }
        ord_seq = (seq_t)ord64;
    }

    /* final value: φ(n) / ord(7,s) */
    return phi_n / ord_seq;
}
