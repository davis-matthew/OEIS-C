/*
 * A276634  Sum of cubes of proper divisors of n.
 * https://oeis.org/A276634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum of cubes of proper divisors of n
 * Range:     u8 n <= 11, u16 n <= 71, u32 n <= 2819, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, sigma3_term
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/sigma3_term.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;  /* by definition a(1)=0 */

    /* factorize n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* product of sigma3 terms for each prime power */
    seq_t sigma = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        sigma = sigma * sigma3_term(primes[i], exps[i]);
    }

    /* n^3 */
    seq_t nsq = (seq_t)n * (seq_t)n;
    seq_t nsq3 = nsq * (seq_t)n;

    /* sum of cubes of proper divisors = sigma - n^3 */
    return sigma - nsq3;
}
