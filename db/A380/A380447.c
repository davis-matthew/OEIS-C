/*
 * A380447  Number of primes dividing n plus n times the number of nonprimes
 *          dividing n.
 * https://oeis.org/A380447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*tau(n) - (n-1)*omega(n)
 * Range:     u8 n <= 47, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where tau counts all divisors and omega counts distinct prime factors */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"   /* provides factor_u64 */

seq_t a(idx_t n) {
    /* Convert index to 64‑bit for factoring */
    uint64_t num = (uint64_t)n;

    /* Temporary storage for prime factors and their exponents */
    /* The maximum number of prime factors for a 64‑bit number is < 20 */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    /* Factorise n */
    factor_u64(num, primes, exps, &cnt);

    /* Compute tau(n) = product of (exponent + 1) */
    seq_t tau = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        tau *= (seq_t)(exps[i] + 1);
    }

    /* Compute omega(n) = number of distinct prime factors */
    seq_t omega = (seq_t)cnt;

    /* a(n) = n*tau - (n-1)*omega */
    seq_t part1 = (seq_t)num * tau;
    seq_t part2 = (seq_t)(num - 1) * omega;
    return part1 - part2;
}
