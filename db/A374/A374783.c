/*
 * A374783  Numerator of the mean unitary abundancy index of the unitary
 *          divisors of n.
 * https://oeis.org/A374783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = numerator of the mean unitary abundancy index of the
 *            unitary divisors of n
 * Range:     u8 n <= 29, u16 n <= 2705, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"

static const uint64_t primes_small[] = {
    2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL,
    31ULL, 37ULL, 41ULL, 43ULL, 47ULL, 53ULL, 59ULL, 61ULL, 67ULL,
    71ULL, 73ULL, 79ULL, 83ULL, 89ULL, 97ULL
};
static const size_t primes_small_cnt = sizeof(primes_small) / sizeof(primes_small[0]);

seq_t a(idx_t n)
{
    /* n >= 1 */
    uint64_t temp = (uint64_t)n;
    seq_t num = 1;
    seq_t den = 1;

    /* factorization by trial division using small primes */
    for (size_t i = 0; i < primes_small_cnt && temp > 1; ++i) {
        uint64_t p = primes_small[i];
        if (p * p > temp) break;
        if (temp % p != 0) continue;

        int exp = 0;
        while (temp % p == 0) {
            temp /= p;
            ++exp;
        }

        /* compute p^exp */
        seq_t pPow = (seq_t)p;
        for (int k = 1; k < exp; ++k) {
            pPow *= (seq_t)p;
        }

        seq_t factor_num = (seq_t)2 * pPow + (seq_t)1;
        seq_t factor_den = (seq_t)2 * pPow;

        num *= factor_num;
        den *= factor_den;
    }

    /* if there is a remaining prime factor > 97 */
    if (temp > 1) {
        uint64_t p = temp;
        seq_t pPow = (seq_t)p;

        seq_t factor_num = (seq_t)2 * pPow + (seq_t)1;
        seq_t factor_den = (seq_t)2 * pPow;

        num *= factor_num;
        den *= factor_den;
    }

    /* reduce the fraction */
    seq_t g = gcd(num, den);
    if (g > 1) {
        num /= g;
        /* den /= g;  // not needed for the return value */
    }

    return num;
}
