/*
 * A323760  Numerator of Product_{d|n} (pod(d)/tau(d)) where pod(k) = the
 *          product of the divisors of k and tau(k) = the number of the
 *          divisors of k.
 * https://oeis.org/A323760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 15, u32 n <= 23, u64 n <= 47, u128 n <= 119,
 *            big any n
 * Helpers:   _factorise_using_spf, big_pow, build_spf, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The implementation evaluates a(n) = Numerator of
 *    ∏_{d|n} ( pod(d) / τ(d) )
 *  where pod(k) = product of all divisors of k and τ(k) = number of
 *  divisors of k.  It uses a least‑prime‑factor table to factorise
 *  all divisors of n and keeps the intermediate fraction in lowest
 *  terms.
 *  The function works for any seq_t/idx_t types supplied by the build.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/gcd.h"
#include "../utils/big_pow.h"
#include "../utils/build_spf.h"
#include "../utils/_factorise_using_spf.h"

/*  Computes the nth term of sequence A323760 (1‑based index).  */
seq_t a(idx_t n)
{
    if (n == 0)                  /* undefined offset – return 1 for safety */
        return (seq_t)1;

    /* Build SPF table up to n */
    idx_t *spf = NULL;
    build_spf(n, &spf);          /* spf[0..n] filled, spf[0] unused */

    seq_t numer   = (seq_t)1;    /* current numerator of product */
    seq_t denom   = (seq_t)1;    /* current denominator of product */

    idx_t primes[64];
    int   exps[64];

    /* Enumerate all divisors d of n */
    for (idx_t d = 1; d <= n; ++d) {
        if (n % d != 0)
            continue;

        /* Factorise d using the SPF table */
        size_t pcnt = _factorise_using_spf(d, primes, exps, spf);

        /* Compute τ(d) = ∏ (e_i + 1) */
        seq_t tau_d = (seq_t)1;
        for (size_t i = 0; i < pcnt; ++i)
            tau_d *= (seq_t)(exps[i] + 1);

        /* Compute pod(d) = ∏ p_i^{e_i * τ(d) / 2} */
        seq_t pod = (seq_t)1;
        for (size_t i = 0; i < pcnt; ++i) {
            seq_t exp = ((seq_t)exps[i] * tau_d) / (seq_t)2;
            seq_t term = big_pow((seq_t)primes[i], exp);
            pod *= term;
        }

        /* Merge the new fraction pod/τ(d) into the running product */
        /* Reduce pod and τ(d) by their gcd */
        seq_t g = gcd(pod, tau_d);
        pod   /= g;
        tau_d /= g;

        /* Reduce numerator with remaining τ(d) */
        g = gcd(numer, tau_d);
        numer /= g;
        tau_d /= g;

        /* Reduce pod with current denominator */
        g = gcd(pod, denom);
        pod   /= g;
        denom /= g;

        /* Multiply into the running fraction */
        numer *= pod;
        denom *= tau_d;

        /* Final simplification */
        g = gcd(numer, denom);
        numer /= g;
        denom /= g;
    }

    free(spf);          /* clean up SPF table */
    return numer;       /* return numerator in lowest terms */
}
