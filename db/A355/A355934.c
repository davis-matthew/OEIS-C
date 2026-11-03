/*
 * A355934  a(n) = sigma(n) / gcd(sigma(n), sigma(A003961(n))), where A003961
 *          is fully multiplicative with a(p) = nextprime(p), and sigma is the
 *          sum of divisors function.
 * https://oeis.org/A355934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = sigma(n) / gcd(sigma(n), sigma(A003961(n)))
 * Range:     u8 n <= 254, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 106 terms through n = 8670 (2026-11-01)
 * Helpers:   factor_u64, gcd, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by factoring n, evaluating sigma(n) with sigma_of, */
/* and computing sigma(A003961(n)) modulo sigma(n) through the geometric */
/* series 1 + p' + p'^2 + … + p'^e for each prime factor, avoiding big integers. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"
#include "../utils/next_prime.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* sigma(n) */
    seq_t sigma_n = sigma_of(n);
    if (sigma_n == 0) return 0;  /* defensive, though sigma(1)=1 */

    /* factor n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t mod = sigma_n;
    seq_t sigma_A_mod = 1;  /* product over factors modulo sigma_n */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        uint64_t pprime = next_prime(p);          /* next prime after p */

        /* compute 1 + pprime + pprime^2 + … + pprime^e modulo mod */
        seq_t sum = 1;
        seq_t pow = 1;
        for (uint64_t j = 1; j <= e; ++j) {
            pow = (pow * (seq_t)pprime) % mod;
            sum = (sum + pow) % mod;
        }

        sigma_A_mod = (sigma_A_mod * sum) % mod;
    }

    /* gcd(sigma_n, sigma_A) = gcd(sigma_n, sigma_A_mod) */
    seq_t g = gcd(sigma_n, sigma_A_mod);

    return sigma_n / g;
}
