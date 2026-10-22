/*
 * A355939  Dirichlet inverse of A080339, characteristic function of
 *          noncomposite numbers.
 * https://oeis.org/A355939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 1, for n = 1; for n > 1, a(n) = - Σ_{p prime; p|n} a(n/p)
 * Range:     i8 n <= 1079, i16 n <= 8616, i32 n <= 8616, i64 n <= 8616,
 *            i128 n <= 8616, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t _a_rec(idx_t n)
{
    if (n == 1)
        return (seq_t)1;                    /* base case */

    seq_t sum = 0;
    idx_t orig = n;
    idx_t tmp  = n;

    /* handle factor 2 */
    if (tmp % 2 == 0) {
        sum += _a_rec(orig / 2);
        while (tmp % 2 == 0)
            tmp /= 2;
    }

    /* odd factors up to sqrt(tmp) */
    for (idx_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            sum += _a_rec(orig / p);
            while (tmp % p == 0)
                tmp /= p;
        }
    }

    /* remaining prime factor, if any */
    if (tmp > 1)
        sum += _a_rec(orig / tmp);

    return -sum;
}

seq_t a(idx_t n)
{
    return _a_rec(n);
}
