/*
 * A327817  Number of irreducible factors in the factorization of the n-th
 *          cyclotomic polynomial over GF(9) (counted with multiplicity).
 * https://oeis.org/A327817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = φ(n) / ord_9(s) where n = 3^e * s, gcd(3, s) = 1. φ(n) is
 *            Euler's totient and ord_9(s) is the multiplicative order of 9
 *            modulo s
 * Range:     u8 n <= 610, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi, factor_u64, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Utility prototypes */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/mod_pow.h"
#include "../utils/calc_phi.h"

/* Compute the multiplicative order of 9 modulo s.
   s is coprime to 3 (i.e. 3∤s).  phi_s is φ(s). */
static seq_t order_of_9(uint64_t s, seq_t phi_s)
{
    if (s == 1)          /* ord_9(1) = 1 */
        return (seq_t)1;

    /* phi_s is small (≤ 2^64-1 for n ≤ 10⁴); cast safely to 64‑bit */
    uint64_t phi_val   = (uint64_t)phi_s;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt = 0;

    factor_u64(phi_val, primes, exps, &cnt);

    uint64_t order = phi_val;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        /* Reduce order by successive factors of p */
        while (order % p == 0) {
            uint64_t cand = order / p;
            seq_t res = mod_pow((seq_t)9, cand, (seq_t)s);
            if (res == (seq_t)1)
                order = cand;
            else
                break;
        }
    }
    return (seq_t)order;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n;
    uint64_t e  = 0;

    while (nn % 3 == 0) {      /* Factor out powers of 3 */
        e++;
        nn /= 3;
    }
    uint64_t s = nn;           /* Remaining part, coprime to 3 */

    /* φ(3^e) = 3^(e‑1)·2 (with φ(1)=1) */
    seq_t phi_3e = 1;
    if (e > 0) {
        seq_t pow3 = 1;
        for (uint64_t i = 1; i < e; ++i)
            pow3 *= 3;
        phi_3e = pow3 * (seq_t)2;
    }

    seq_t phi_s = calc_phi((seq_t)s);        /* φ(s) */
    seq_t phi_n = phi_3e * phi_s;            /* φ(n) = φ(3^e)·φ(s) */

    seq_t ord = order_of_9(s, phi_s);        /* ord_9(s) */

    return phi_n / ord;                      /* a(n) = φ(n) / ord_9(s) */
}
