/*
 * A379028  The number of modified exponential divisors of n.
 * https://oeis.org/A379028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product over prime powers p^e || n of d(e+1), where d(k)
 *            is the number of divisors of k (A000005)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t divisor_count_uint64(uint64_t n)
{
    seq_t res = 1;
    /* count factors of 2 */
    if ((n & 1ULL) == 0) {
        uint64_t c = 0;
        while ((n & 1ULL) == 0) {
            n >>= 1;
            c++;
        }
        res *= (seq_t)(c + 1);
    }
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            uint64_t c = 0;
            while (n % p == 0) {
                n /= p;
                c++;
            }
            res *= (seq_t)(c + 1);
        }
    }
    if (n > 1)
        res *= 2;  /* n is prime, exponent 1 -> (1+1)=2 */
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;      /* undefined, but handle gracefully */

    seq_t result = 1;
    uint64_t m = (uint64_t)n;

    /* factorization of n */
    /* factor 2 */
    uint64_t e = 0;
    while (m % 2ULL == 0) {
        m >>= 1;
        e++;
    }
    if (e > 0) {
        result *= divisor_count_uint64(e + 1);
    }

    /* odd factors */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            e = 0;
            while (m % p == 0) {
                m /= p;
                e++;
            }
            result *= divisor_count_uint64(e + 1);
        }
    }

    /* remaining prime factor > sqrt */
    if (m > 1) {
        /* exponent is 1 */
        result *= divisor_count_uint64(2);  /* (1+1)=2 */
    }

    return result;
}
