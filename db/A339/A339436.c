/*
 * A339436  If n = p_1 * ... * p_m with primes p_i <= p_{i+1}, a(n) =
 *          Sum_{j=1..m-1} p_1*...*p_j + Sum_{j=2..m} p_j*...*p_m.
 * https://oeis.org/A339436
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   sum of proper prefixes and suffixes of the prime factorization
 *            of n. If n is prime the value is 0
 * Range:     u8 n <= 191, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_u64.h"
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    /* Pull the prime factorization.  factor_u64 returns each prime
       once together with its exponent. */
    uint64_t primes[64];
    uint64_t exponents[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exponents, &cnt);

    /* Build the full list with multiplicity (e.g. 12 -> {2,2,3}).
       The maximum number of prime factors for 64‑bit numbers is < 64. */
    uint64_t list[64];
    size_t m = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exponents[i];
        for (uint64_t j = 0; j < e; ++j) {
            list[m++] = p;
        }
    }

    if (m <= 1)   /* prime (or n=1, but n<2 handled earlier) */
        return 0;

    seq_t sum  = 0;
    seq_t prod = 1;

    /* Sum of proper prefixes: j = 1 .. m-1 */
    for (size_t i = 0; i + 1 < m; ++i) {
        prod *= (seq_t)list[i];
        sum  += prod;
    }

    /* Sum of proper suffixes: j = 2 .. m */
    prod = 1;
    for (size_t i = m - 1; i > 0; --i) {
        prod *= (seq_t)list[i];
        sum  += prod;
    }

    return sum;
}
