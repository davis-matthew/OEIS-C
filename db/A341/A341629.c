/*
 * A341629  Characteristic function of A055932: a(n) = 1 if n is a number all
 *          of whose prime divisors are consecutive primes starting at 2,
 *          otherwise 0.
 * https://oeis.org/A341629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is a product of consecutive primes starting at 2,
 *            otherwise 0. The function tests the set of distinct prime
 *            factors of n; if the smallest factor is 2 and each subsequent
 *            factor is the next prime after the previous one, a(n)=1, else
 *            a(n)=0
 * Range:     u8 n <= 9290, u16 n <= 9290, u32 n <= 9290, u64 n <= 9290,
 *            u128 n <= 9290, big any n
 * Helpers:   factor_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    /* trivial cases */
    if (n <= 1)
        return (seq_t)1;  /* a(1)=1, a(0) undefined but safe */

    /* factorization */
    uint64_t primes[64], exps[64];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* n>1 always has at least one factor */
    if (cnt == 0)
        return (seq_t)1;  /* should not happen, but be safe */

    /* must start with 2 */
    if (primes[0] != 2)
        return (seq_t)0;

    /* consecutive primes check */
    for (size_t i = 1; i < cnt; ++i) {
        uint64_t expected = next_prime(primes[i - 1]);
        if (expected != primes[i])
            return (seq_t)0;
    }
    return (seq_t)1;
}
