/*
 * A355932  a(n) = gcd(sigma(n), sigma(A003961(n))), where A003961 is fully
 *          multiplicative with a(p) = nextprime(p).
 * https://oeis.org/A355932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(sigma(n), sigma(A003961(n)))
 * Range:     u8 n <= 16, u16 n <= 3777, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where A003961 is multiplicative with a(p) = nextprime(p) */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* offset starts at 1; undefined for n<1 */

    seq_t sigma_n = 1;  /* sigma(n) */
    seq_t sigma_m = 1;  /* sigma(A003961(n)) */
    idx_t temp = n;

    /* factor n by trial division */
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            /* count exponent of p */
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            /* sigma(p^e) = (p^(e+1)-1)/(p-1) */
            seq_t p_pow = 1;
            for (idx_t i = 0; i < e + 1; ++i) p_pow *= (seq_t)p;
            seq_t sigma_pe = (p_pow - 1) / ((seq_t)p - 1);

            /* next prime after p */
            uint64_t nextp = next_prime(p);
            seq_t np = (seq_t)nextp;
            seq_t np_pow = 1;
            for (idx_t i = 0; i < e + 1; ++i) np_pow *= np;
            seq_t sigma_np = (np_pow - 1) / (np - 1);

            sigma_n *= sigma_pe;
            sigma_m *= sigma_np;
        }
    }

    /* remaining prime factor if any */
    if (temp > 1) {
        idx_t p = temp;
        idx_t e = 1;

        seq_t p_pow = (seq_t)p * (seq_t)p;  /* p^(e+1) */
        seq_t sigma_pe = (p_pow - 1) / ((seq_t)p - 1);

        uint64_t nextp = next_prime(p);
        seq_t np = (seq_t)nextp;
        seq_t np_pow = np * np;
        seq_t sigma_np = (np_pow - 1) / (np - 1);

        sigma_n *= sigma_pe;
        sigma_m *= sigma_np;
    }

    return gcd(sigma_n, sigma_m);
}
