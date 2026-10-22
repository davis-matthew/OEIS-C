/*
 * A349381  Dirichlet convolution of A003961 with A349125 (Dirichlet inverse
 *          of A064989), where A003961 and A064989 are fully multiplicative
 *          sequences that shift the prime factorization of n one step towards
 *          larger and smaller primes respectively.
 * https://oeis.org/A349381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{d|n} A003961(n/d) * A349125(d) * A003961(p^e) =
 *            next_prime(p)^e * A349125(n) = 0 if n has a squared factor, *
 *            else (-1)^ω(n) * product(prev_prime(p)) * Therefore for a prime
 *            power p^e (e≥1) * a(p^e) = (next_prime(p) – prev_prime(p)) *
 *            next_prime(p)^(e-1) * a(1) = 1
 * Range:     u8 n <= 63, u16 n <= 2047, u32 n <= 9982, u64 n <= 9982,
 *            u128 n <= 9982, big any n
 * Helpers:   factor_u64, next_prime, prev_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"
#include "../utils/prev_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined below offset */
    if (n == 1) return 1;          /* base case */

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        seq_t q = (seq_t)next_prime(p);
        seq_t r = (p == 2 ? (seq_t)1 : (seq_t)prev_prime(p));

        /* compute next_prime(p)^(e-1) */
        seq_t qpow = 1;
        for (uint64_t k = 1; k < e; ++k) {
            qpow *= q;
        }

        seq_t part = (q - r) * qpow;
        result *= part;
    }

    return result;
}
