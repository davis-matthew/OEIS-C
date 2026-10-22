/*
 * A369911  a(n) = Sum_{p|n, p prime} p * sopf(n/p).
 * https://oeis.org/A369911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{p|n, p prime} p * sopf(n/p), where sopf(x) = sum of
 *            distinct prime divisors of x
 * Range:     u8 n <= 128, u16 n <= 9970, u32 n <= 9970, u64 n <= 9970,
 *            u128 n <= 9970, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    /* factor n by trial division */
    uint64_t temp = (uint64_t)n;
    uint64_t primes[64];
    int exps[64];
    size_t cnt = 0;

    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[cnt]   = p;
            exps[cnt]     = e;
            ++cnt;
        }
    }
    if (temp > 1) {
        primes[cnt]   = temp;
        exps[cnt]     = 1;
        ++cnt;
    }

    /* sum of distinct prime divisors of n */
    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i)
        sum += (seq_t)primes[i];

    /* accumulate the convolution */
    seq_t res = 0;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p   = (seq_t)primes[i];
        seq_t sub = (exps[i] == 1) ? p : (seq_t)0;
        seq_t term = p * (sum - sub);
        res += term;
    }

    return res;
}
