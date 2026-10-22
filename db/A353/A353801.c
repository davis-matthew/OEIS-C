/*
 * A353801  a(n) = A049559(n) / A187730(n), where A049559(n) and A187730(n)
 *          are the greatest common divisors between Euler phi(n) and n-1, and
 *          between Carmichael lambda(n) and n-1, respectively.
 * https://oeis.org/A353801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(phi(n), n-1) / gcd(lambda(n), n-1)
 * Range:     u8 n <= 2267, u16 n <= 9744, u32 n <= 9744, u64 n <= 9744,
 *            u128 n <= 9744, big any n
 * Helpers:   factor_u64, gcd, lambda_prime_power
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"
#include "../utils/lambda_prime_power.h"

static uint64_t s_primes[64];
static uint64_t s_exps[64];

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* sequence is defined for n≥1 */

    uint64_t N = (uint64_t)n;

    /* factor N */
    size_t cnt = 0;
    factor_u64(N, s_primes, s_exps, &cnt);

    /* φ(N) */
    seq_t phi = (seq_t)N;
    for (size_t i = 0; i < cnt; ++i)
        phi -= phi / (seq_t)s_primes[i];

    /* λ(N) as lcm of prime‑power components */
    seq_t lambda = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t term = lambda_prime_power(s_primes[i], s_exps[i]);
        seq_t g = gcd(lambda, term);
        lambda = (lambda / g) * term;
    }

    seq_t n1 = (seq_t)(N - 1);
    seq_t g1 = gcd(phi, n1);
    seq_t g2 = gcd(lambda, n1);
    return g1 / g2;
}
