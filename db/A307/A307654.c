/*
 * A307654  a(n) = Product_{p|n, p prime} (1 - p^p).
 * https://oeis.org/A307654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = Π_{p|n, p prime} (1 - p^p)
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 10, i64 n <= 16, i128 n <= 28,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)1;          /* a(1) = 1 */

    seq_t result = (seq_t)1;
    uint64_t m = (uint64_t)n;

    /* handle factor 2 */
    if ((m & 1ULL) == 0) {
        seq_t pow = big_pow((seq_t)2, (seq_t)2);    /* 2^2 */
        result *= ((seq_t)1 - pow);
        while ((m & 1ULL) == 0) m >>= 1;
    }

    /* odd prime factors */
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            seq_t pow = big_pow((seq_t)p, (seq_t)p);    /* p^p */
            result *= ((seq_t)1 - pow);
            while (m % p == 0) m /= p;
        }
    }

    /* remaining prime factor > sqrt(n) */
    if (m > 1) {
        seq_t pow = big_pow((seq_t)m, (seq_t)m);       /* m^m */
        result *= ((seq_t)1 - pow);
    }

    return result;
}
