/*
 * A355934  a(n) = sigma(n) / gcd(sigma(n), sigma(A003961(n))), where A003961
 *          is fully multiplicative with a(p) = nextprime(p), and sigma is the
 *          sum of divisors function.
 * https://oeis.org/A355934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = sigma(n) / gcd(sigma(n), sigma(A003961(n)))
 * Range:     u8 n <= 254, u16 n <= 8670, u32 n <= 8670, u64 n <= 8670,
 *            u128 n <= 8670, big any n
 * Helpers:   factor_u64, next_prime, seq_gcd, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by factoring n, evaluating sigma(n) with sigma_of, */
/* and computing sigma(A003961(n)) modulo sigma(n) through the geometric */
/* series 1 + p' + p'^2 + … + p'^e for each prime factor, avoiding big integers. */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"
#include "../utils/sigma_of.h"
#include "../utils/seq_gcd.h"

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
    seq_t g = seq_gcd(sigma_n, sigma_A_mod);

    return sigma_n / g;
}
