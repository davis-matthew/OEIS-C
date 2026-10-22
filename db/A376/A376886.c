/*
 * A376886  The number of distinct factors of n of the form p^(k!), where p is
 *          a prime and k >= 1, when the factorization is uniquely done using
 *          the factorial-base representation of the exponents in the prime
 *          factorization of n.
 * https://oeis.org/A376886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = sum_{p^e || n} A060130(e) where A060130(e) is the number
 *            of non‑zero factorial‑base digits of e
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* precompute factorials up to 20! (20! < 2^64) */
static const idx_t FACT[21] = {
    1ULL, 1ULL, 2ULL, 6ULL, 24ULL,
    120ULL, 720ULL, 5040ULL, 40320ULL, 362880ULL,
    3628800ULL, 39916800ULL, 479001600ULL,
    6227020800ULL, 87178291200ULL, 1307674368000ULL,
    20922789888000ULL, 355687428096000ULL,
    6402373705728000ULL, 121645100408832000ULL, 2432902008176640000ULL
};

/* count non‑zero factorial‑base digits of e (e > 0) */
static unsigned int factorial_digit_count(idx_t e)
{
    if (e == 0) return 0;
    unsigned int count = 0;
    for (int k = 20; k >= 1; --k) {
        if (FACT[k] <= e) {
            idx_t d = e / FACT[k];
            if (d > 0) ++count;
            e -= d * FACT[k];
        }
    }
    return count;
}

seq_t a(idx_t n)
{
    if (n <= 1) return 0;

    idx_t tmp = n;
    unsigned int total = 0;
    unsigned int exp;

    /* factor 2 */
    if ((tmp & 1ULL) == 0) {
        exp = 0;
        while ((tmp & 1ULL) == 0) {
            tmp >>= 1;
            ++exp;
        }
        total += factorial_digit_count(exp);
    }

    /* odd factors */
    for (idx_t p = 3; p <= tmp / p; p += 2) {
        if (tmp % p == 0) {
            exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            total += factorial_digit_count(exp);
        }
    }

    /* residual prime factor > 1 */
    if (tmp > 1) {
        total += 1; /* exponent is 1 */
    }

    return (seq_t)total;
}
