/*
 * A365101  Number of distinct residues of x^n (mod n^4), x=0..n^4-1.
 * https://oeis.org/A365101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct residues of x^n modulo n^4 (i.e. the
 *            size of the image of the n-th power map on ℤ/(n⁴)ℤ)
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   pow2, seq_gcd, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/seq_gcd.h"
#include "../utils/seq_pow.h"
#include "../utils/pow2.h"

/* Size of the image of the exponentiation map x↦x^n on the unit
   group U(p^m) (modulo p^m).  The exponent n is given as an idx_t. */
static seq_t image_units_size(idx_t n, uint64_t p, idx_t m)
{
    /* For p = 2 special handling of the non‑cyclic group structure */
    if (p == 2U) {
        if (m == 1U)       /* U(2) = {1} */
            return (seq_t)1;

        if (m == 2U) {     /* U(4) ≅ C₂ */
            seq_t phi = (seq_t)2;
            seq_t d = seq_gcd(phi, (seq_t)n);
            return phi / d;
        }

        /* m ≥ 3:  U(2^m) ≅ C₂ × C_{2^{m‑2}}  */
        seq_t sizeC2 = ((n & 1U) == 0) ? (seq_t)1 : (seq_t)2;
        seq_t exp_part = pow2((idx_t)(m - 2));  /* 2^(m‑2)  */
        seq_t d = seq_gcd(exp_part, (seq_t)n);
        seq_t sizeC = exp_part / d;
        return sizeC2 * sizeC;
    }

    /* p is an odd prime: U(p^m) is cyclic of order φ = p^{m‑1}(p‑1) */
    seq_t pow_part = seq_pow((seq_t)p, (idx_t)m - 1U);   /* p^{m‑1}  */
    seq_t phi = pow_part * (seq_t)(p - 1U);
    seq_t d = seq_gcd(phi, (seq_t)n);
    return phi / d;
}

seq_t a(idx_t n)
{
    if (n == 1U)          /* n^4 = 1, only residue 0 */
        return (seq_t)1;

    /*--- Prime factorisation of n -------------------------------------*/
    uint64_t primes[64];
    int      exps[64];
    size_t   pcnt = 0;
    uint64_t tmp = n;
    for (uint64_t p = 2U; p * p <= tmp; ++p) {
        if (tmp % p == 0U) {
            int e = 0;
            while (tmp % p == 0U) {
                tmp /= p;
                ++e;
            }
            primes[pcnt] = p;
            exps[pcnt]  = e;
            ++pcnt;
        }
    }
    if (tmp > 1U) {    /* remaining prime factor */
        primes[pcnt] = tmp;
        exps[pcnt]  = 1;
        ++pcnt;
    }

    /*--- Image size for each prime power --------------------------------*/
    seq_t total = (seq_t)1;            /* product of the per‑prime sizes */

    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = primes[i];
        int      e = exps[i];
        idx_t    k = 4U * (idx_t)e;    /* exponent of p in n⁴ : p^(4e) */

        seq_t size_p = (seq_t)1;      /* residue 0 modulo p^(4e) */

        /* t : exponent of p in x (v_p(x))  */
        for (idx_t t = 0; ; ++t) {
            idx_t prod = t * n;
            if (prod >= k)
                break;
            idx_t m = k - prod;      /* exponent of modulus after removing p^(t n) */
            seq_t I = image_units_size(n, p, m);
            size_p += I;
        }

        total *= size_p;
    }

    return total;
}
