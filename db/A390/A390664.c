/*
 * A390664  The sum of the divisors of the smallest number whose cube is
 *          divisible by n.
 * https://oeis.org/A390664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = A000203(A019555(n)) i.e. a(n) = product_{p^e||n}
 *            (p^(ceil(e/3)+1)-1)/(p-1)
 * Range:     u8 n <= 16, u16 n <= 3315, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses big_pow from the utilities */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }

    uint64_t m = (uint64_t)n;        /* factorization target */
    seq_t   res = 1;                 /* accumulate sigma(k) */

    /* factor 2 separately */
    if ((m & 1) == 0) {
        uint64_t e = 0;
        while ((m & 1) == 0) {
            m >>= 1;
            ++e;
        }
        uint64_t t = (e + 2) / 3;    /* ceil(e/3) */
        seq_t pow = big_pow((seq_t)2, (seq_t)(t + 1));
        seq_t factor = (pow - 1) / (seq_t)(2 - 1);
        res *= factor;
    }

    /* odd primes */
    uint64_t p;
    for (p = 3; p <= m / p; p += 2) {   /* avoid overflow */
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            uint64_t t = (e + 2) / 3;   /* ceil(e/3) */
            seq_t pow = big_pow((seq_t)p, (seq_t)(t + 1));
            seq_t factor = (pow - 1) / ((seq_t)p - 1);
            res *= factor;
        }
    }

    /* remaining prime factor, if any */
    if (m > 1) {
        uint64_t e = 1;                /* exponent is 1 */
        uint64_t t = (e + 2) / 3;     /* ceil(1/3) = 1 */
        seq_t pow = big_pow((seq_t)m, (seq_t)(t + 1));
        seq_t factor = (pow - 1) / ((seq_t)m - 1);
        res *= factor;
    }

    return res;
}
