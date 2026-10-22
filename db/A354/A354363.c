/*
 * A354363  a(n) = LCM_{p^e||n} (q^(e+1)-1)/(q-1), when n = Product_{p^e||n},
 *          with each p^e the maximal power of prime p that divides n, and q =
 *          nextPrime(p).
 * https://oeis.org/A354363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = LCM_{p^e||n} (q^(e+1)-1)/(q-1), where q = nextPrime(p).
 *            For n = 1, a(1) = 1
 * Range:     u8 n <= 12, u16 n <= 1023, u32 n <= 9311, u64 n <= 9311,
 *            u128 n <= 9311, big any n
 * Helpers:   factor_u64, next_prime, seq_gcd, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/next_prime.h"
#include "../utils/factor_u64.h"
#include "../utils/seq_gcd.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but guard */
    /* factorise n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;              /* LCM accumulator */
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        uint64_t q = next_prime(p);                 /* next prime after p */

        seq_t pow = seq_pow((seq_t)q, (idx_t)(e + 1));   /* q^(e+1) */
        seq_t val = (pow - 1) / (seq_t)(q - 1);          /* (q^(e+1)-1)/(q-1) */

        seq_t g   = seq_gcd(result, val);               /* gcd(result,val) */
        seq_t tmp = result / g;                         /* result/gcd */
        result = tmp * val;                             /* lcm part */
    }

    return result;
}
