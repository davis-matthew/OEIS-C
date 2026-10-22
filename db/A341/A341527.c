/*
 * A341527  Denominator of ratio n*sigma(A003961(n)) / sigma(n)*A003961(n),
 *          where sigma is the sum of divisors of n, and A003961 shifts the
 *          prime factorization of n one step towards larger primes.
 * https://oeis.org/A341527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of n*sigma(A003961(n)) /
 *            (sigma(n)*A003961(n))
 * Range:     u8 n <= 7, u16 n <= 63, u32 n <= 4095, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   gcd, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    /* factorise n by trial division (n ≤ 10 000, so this is cheap) */
    uint64_t temp   = (uint64_t)n;
    uint64_t primes[16];
    uint64_t exps[16];
    size_t   cnt    = 0;

    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            uint64_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[cnt] = p;
            exps[cnt]   = e;
            ++cnt;
        }
    }
    if (temp > 1) {
        primes[cnt] = temp;
        exps[cnt]   = 1;
        ++cnt;
    }

    /* build A003961(n) and the two sigma values */
    seq_t B = 1;        /* shifted value */
    seq_t sigma_n = 1;  /* sigma(n) */
    seq_t sigma_B = 1;  /* sigma(A003961(n)) */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p  = primes[i];
        uint64_t e  = exps[i];
        uint64_t np = next_prime(p);   /* next prime after p */

        /* contribute building B = ∏ np^e */
        seq_t np_seq = (seq_t)np;
        for (uint64_t k = 0; k < e; ++k)
            B *= np_seq;

        /* sigma(np^e) = (np^(e+1)-1)/(np-1) */
        seq_t pow_np = 1;
        for (uint64_t k = 0; k <= e; ++k)
            pow_np *= np_seq;
        seq_t sigma_np = (pow_np - 1) / (np_seq - 1);
        sigma_B *= sigma_np;

        /* sigma(p^e) for the original number */
        seq_t p_seq = (seq_t)p;
        seq_t pow_p = 1;
        for (uint64_t k = 0; k <= e; ++k)
            pow_p *= p_seq;
        seq_t sigma_p = (pow_p - 1) / (p_seq - 1);
        sigma_n *= sigma_p;
    }

    /* compute the fraction n*sigma_B / (sigma_n*B) */
    seq_t num = (seq_t)n * sigma_B;
    seq_t den = sigma_n * B;

    seq_t g = gcd(num, den);
    return den / g;
}
