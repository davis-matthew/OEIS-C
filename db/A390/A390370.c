/*
 * A390370  The sum of the infinitary divisors of n that are exponentially odd
 *          numbers (A268335).
 * https://oeis.org/A390370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult, changed
 * Formula:   a(n) = ∏p^e‖n, * if e is even then factor 1, * if e is odd then
 *            factor 1 + p * isigma(p^(e-1)) * where isigma(p^k) =
 *            ∏_{b∈bits(k)} (1 + p^(2^b)) is the sum of * infinitary divisors
 *            of the prime power
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isigma_prime_power
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/isigma_prime_power.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                /* not defined for n = 0 */

    uint64_t m = (uint64_t)n;
    uint64_t primes[64];
    uint32_t exps[64];
    size_t cnt = 0;
    uint64_t t = m;

    /* trial‑division factorisation – fast enough for n ≤ ~10⁴ */
    for (uint64_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            uint32_t e = 0;
            while (t % p == 0) {
                t /= p;
                ++e;
            }
            primes[cnt] = p;
            exps[cnt]   = e;
            ++cnt;
        }
    }
    if (t > 1) {                /* remaining prime factor */
        primes[cnt] = t;
        exps[cnt]   = 1;
        ++cnt;
    }

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint32_t e = exps[i];
        if ((e & 1) == 0)     /* even exponent → factor 1 */
            continue;
        seq_t isig   = isigma_prime_power(p, (uint64_t)(e - 1));
        seq_t factor = (seq_t)1 + ((seq_t)p) * isig;
        result *= factor;
    }

    return result;
}
