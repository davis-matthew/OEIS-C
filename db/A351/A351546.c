/*
 * A351546  a(n) is the largest unitary divisor of sigma(n) coprime with
 *          A003961(n), where A003961 is fully multiplicative with a(p) =
 *          nextprime(p), and sigma is the sum of divisors function.
 * https://oeis.org/A351546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{q^f || σ(n)} q^f for all primes q that do NOT
 *            divide ∏_{p||n} nextprime(p)
 * Range:     u8 n <= 16, u16 n <= 3507, u32 n <= 9635, u64 n <= 9635,
 *            u128 n <= 9635, big any n
 * Helpers:   factor_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"

seq_t a(idx_t n) {
    /* handle n = 0 (outside stated domain) */
    if (n == 0) return 0;

    /* Factor n */
    uint64_t primes[32];
    uint64_t exps[32];
    size_t pcnt;
    factor_u64((uint64_t)n, primes, exps, &pcnt);

    /* Compute the next prime for each prime divisor of n */
    uint64_t nextp[32];
    for (size_t i = 0; i < pcnt; ++i)
        nextp[i] = next_prime(primes[i]);

    /* Compute σ(n) multiplicatively */
    seq_t sigma = 1;
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        /* p^(e+1) */
        seq_t pow = 1;
        for (uint64_t k = 0; k < e + 1; ++k)
            pow *= (seq_t)p;

        /* (p^(e+1)-1)/(p-1) */
        seq_t term = (pow - 1) / (seq_t)(p - 1);
        sigma *= term;
    }

    /* Factor σ(n) (σ(n) fits in 64‑bit for n ≤ 10⁴, so cast safely) */
    uint64_t sig = (uint64_t)sigma;
    uint64_t sigpr[32];
    uint64_t sigex[32];
    size_t scnt;
    factor_u64(sig, sigpr, sigex, &scnt);

    /* Build result: product of prime powers in σ(n) that are NOT among nextp[] */
    seq_t result = 1;
    for (size_t i = 0; i < scnt; ++i) {
        uint64_t q = sigpr[i];
        bool exclude = false;
        for (size_t j = 0; j < pcnt; ++j) {
            if (q == nextp[j]) { exclude = true; break; }
        }
        if (!exclude) {
            seq_t mul = 1;
            for (uint64_t k = 0; k < sigex[i]; ++k)
                mul *= (seq_t)q;
            result *= mul;
        }
    }
    return result;
}
