/*
 * A276653  a(n) = floor(Sum_{p|n} 0.p) where p runs through the prime
 *          divisors of n>1.
 * https://oeis.org/A276653
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = floor( sum_{p|n} 0.p )
 *   where the sum runs over the distinct prime divisors of n.
 *   0.p denotes the decimal fraction consisting of p written after
 *   the decimal point (e.g. 0.11 = 11/100).
 */
#include <stdint.h>
#include "../utils/digit_count.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;

    uint64_t temp = (uint64_t)n;
    uint64_t primes[64];
    int pcnt = 0;

    /* factor 2 */
    if ((temp & 1ULL) == 0) {
        primes[pcnt++] = 2;
        while ((temp & 1ULL) == 0) temp >>= 1;
    }

    /* trial division by odd numbers */
    for (idx_t i = 3; i * i <= temp; i += 2) {
        if (temp % i == 0) {
            primes[pcnt++] = i;
            while (temp % i == 0) temp /= i;
        }
    }

    /* remaining prime factor > 1 */
    if (temp > 1) {
        primes[pcnt++] = temp;
    }

    /* find maximum number of decimal digits among the prime factors */
    int maxd = 0;
    for (int k = 0; k < pcnt; ++k) {
        int d = digit_count(primes[k]);
        if (d > maxd) maxd = d;
    }

    /* common denominator 10^maxd */
    seq_t scale = 1;
    for (int i = 0; i < maxd; ++i) scale *= 10;

    /* accumulate the numerator of the sum */
    seq_t sum = 0;
    for (int k = 0; k < pcnt; ++k) {
        uint64_t p = primes[k];
        int d = digit_count(p);
        seq_t term = (seq_t)p;
        int shift = maxd - d;
        for (int s = 0; s < shift; ++s) term *= 10;
        sum += term;
    }

    /* floor of the sum is the integer part of sum/scale */
    return sum / scale;
}
