/*
 * A327816  Number of irreducible factors in the factorization of the n-th
 *          cyclotomic polynomial over GF(8) (counted with multiplicity).
 * https://oeis.org/A327816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = φ(n) / ord(8, s), where n = 2^e * s, gcd(2,s)=1
 * Range:     u8 n <= 271, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, mod_pow, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/phi.h"
#include "../utils/mod_pow.h"
#include "../utils/factor_u64.h"

/* Compute the multiplicative order of 8 modulo the odd integer s.
   Returns the order as a seq_t value. */
static seq_t ord8(uint64_t s, uint64_t phi_s)
{
    if (s == 1) return (seq_t)1;          /* ord(8,1) = 1 */

    uint64_t d = phi_s;   /* d will be reduced to the minimal order */

    uint64_t primes[64], exps[64];
    size_t cnt;
    factor_u64(phi_s, primes, exps, &cnt);   /* factor φ(s) */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        while (d % p == 0) {
            uint64_t cand = d / p;
            /* check whether 8^cand ≡ 1 (mod s) */
            if (mod_pow((seq_t)8, cand, (seq_t)s) == (seq_t)1)
                d = cand;        /* reduce d */
            else
                break;           /* cannot reduce further with this prime factor */
        }
    }
    return (seq_t)d;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* offset 1, behaviour for n<1 is irrelevant */

    /* φ(n) */
    seq_t phi_n = phi(n);

    /* odd part s of n (remove all factors of 2) */
    uint64_t temp = (uint64_t)n;
    while ((temp & 1ULL) == 0) temp >>= 1;
    uint64_t s = temp;

    /* φ(s) as an unsigned 64-bit integer (s ≤ n ≤ 10⁴, φ(s) ≤ 4000) */
    seq_t phi_s_seq = phi((idx_t)s);
    uint64_t phi_s = (uint64_t)phi_s_seq;

    seq_t ord = ord8(s, phi_s);
    return phi_n / ord;                /* integer division as per formula */
}
