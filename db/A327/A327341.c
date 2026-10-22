/*
 * A327341  Denominators of the rationals r(n) = (1/n^2)*Phi_1(n), with
 *          Phi_1(n) = Sum{k=1..n} psi(k), with the Dedekind psi function.
 * https://oeis.org/A327341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   build_spf, gcd, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A327341 – Denominators of r(n) = (1/n²) * Φ₁(n),
   where Φ₁(n) = Σ_{k=1..n} ψ(k) and ψ(k) = k·∏_{p|k}(1+1/p).  */

/*  We use the identity:   r(n) = S(n)/n²,  S(n) = Σ ψ(k).
 *  So a(n) = denominator(r(n)) = n² / gcd(S(n), n²).  */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/pow_uint.h"
#include "../utils/gcd.h"
#include "../utils/build_spf.h"

/* static storage for smallest prime factors up to the current maximum n */
static idx_t *spf = NULL;
static idx_t  spf_limit = 0;

/* compute ψ(k) modulo mod, using precomputed smallest prime factors */
static seq_t psi_mod(idx_t k, seq_t mod)
{
    seq_t prod = 1;           /* running product modulo mod   */
    idx_t temp = k;           /* remaining part to factorise   */

    while (temp > 1) {
        uint64_t p = spf[temp];          /* smallest prime factor */
        idx_t e  = 0;                    /* exponent of p in k    */
        while (temp % p == 0) {
            temp /= p;
            ++e;
        }
        /* part = p^(e-1) * (p+1)   (mod mod)   */
        seq_t part = pow_uint(p, e == 0 ? 0 : e - 1);
        part *= (p + 1);
        part %= mod;
        prod *= part;
        prod %= mod;
    }
    return prod;
}

seq_t a(idx_t n)
{
    if (n == 0)                 /* undefined for n=0 – return 0 as placeholder */
        return (seq_t)0;

    /* Ensure SPF table is built up to n */
    if (n > spf_limit) {
        free(spf);
        build_spf(n, &spf);
        spf_limit = n;
    }

    /* mod = n²  (maximum value fits in seq_t) */
    seq_t mod = (seq_t)n;
    mod *= (seq_t)n;

    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t term = psi_mod(k, mod);
        sum += term;
        sum %= mod;
    }

    seq_t g = gcd(sum, mod);
    seq_t denom = mod / g;
    return denom;
}
