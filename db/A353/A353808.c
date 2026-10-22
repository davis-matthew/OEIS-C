/*
 * A353808  a(n) = 1 if A187730(n) < A049559(n), otherwise 0. Here A187730(n)
 *          and A049559(n) are the greatest common divisors between Carmichael
 *          lambda(n) and n-1, and between Euler phi(n) and n-1, respectively.
 * https://oeis.org/A353808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if gcd(λ(n), n-1) < gcd(φ(n), n-1); otherwise 0
 * Range:     u8 n <= 18, u16 n <= 3066, u32 n <= 9022, u64 n <= 9022,
 *            u128 n <= 9022, big any n
 * Helpers:   build_primes_upto, factorize, gcd, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* λ is the Carmichael function, φ is Euler's totient. */

#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/phi.h"
#include "../utils/build_primes_upto.h"
#include "../utils/factorize.h"

seq_t a(idx_t n)
{
    /* Handle n = 0 or 1 in the same uniform way */
    if (n == 0) return 0;   /* not used, but safe */

    /* Generate all primes ≤ n */
    uint64_t *prime_list = NULL;
    size_t   prime_cnt  = 0;
    build_primes_upto(n, &prime_list, &prime_cnt);

    /* Allocate array for exponents of the factorisation */
    int *exps = NULL;
    if (prime_cnt > 0)
    {
        exps = (int *)malloc(prime_cnt * sizeof(int));
        if (!exps) {
            free(prime_list);
            return 0;      /* memory failure – safe default */
        }
    }

    /* Factorise n; factorize replaces primes[] with the prime factors
     * of n (in the first `fac_cnt` positions). */
    size_t fac_cnt = factorize(n, prime_list, exps);

    /* Compute λ(n) via the Chinese remainder theorem for prime powers */
    seq_t lambda = 1;
    for (size_t i = 0; i < fac_cnt; ++i)
    {
        uint64_t p = prime_list[i];
        int      e = exps[i];
        seq_t part = 1;

        if (p == 2u)
        {
            /* λ(2^e) */
            if (e == 1)          part = 1;
            else if (e == 2)     part = 2;
            else                 part = (seq_t)4 << (e - 3);  /* 2^{e-2} */
        }
        else
        {
            /* λ(p^e) = p^{e-1} * (p-1) */
            for (int j = 1; j < e; ++j)
                part *= (seq_t)p;        /* multiply by p, e-1 times */
            part *= (p - 1u);
        }

        /* λ(n) = lcm of all parts */
        seq_t g = gcd(lambda, part);
        lambda = (lambda / g) * part;
    }

    /* Compute φ(n) using the provided utility */
    seq_t phi_val = phi(n);

    /* Compute the two gcds with n-1 */
    seq_t n_minus_1 = (seq_t)(n - 1);
    seq_t g1 = gcd(lambda, n_minus_1);      /* A187730(n) */
    seq_t g2 = gcd(phi_val, n_minus_1);     /* A049559(n) */

    /* Clean up */
    free(prime_list);
    free(exps);

    /* Return 1 if λ(n) < φ(n) gcds inequality holds, else 0 */
    return (seq_t)(g1 < g2 ? 1 : 0);
}
