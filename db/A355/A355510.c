/*
 * A355510  a(n) is the number of monic polynomials of degree n over GF(7)
 *          without linear factors.
 * https://oeis.org/A355510
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=0}^{min(7,n)} (-1)^k * binomial(7,k) * 7^{n-k},
 *            with a(0) = 0. For n ≥ 7 this simplifies to a(n) = 6^7 * 7^{n-7}
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   binomial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow_uint.h"

static const unsigned long long POW7[8] = {
    1ULL,                 /* 7^0 */
    7ULL,                 /* 7^1 */
    49ULL,                /* 7^2 */
    343ULL,               /* 7^3 */
    2401ULL,              /* 7^4 */
    16807ULL,             /* 7^5 */
    117649ULL,            /* 7^6 */
    823543ULL             /* 7^7 (unused for n<7) */
};

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    if (n < 8) {
        unsigned long long sum = 0;
        for (idx_t k = 0; k <= n; ++k) {
            unsigned long long term =
                (unsigned long long)binomial(7, k) * POW7[n - k];
            if ((k & 1ULL) == 0ULL)
                sum += term;
            else
                sum -= term;
        }
        return (seq_t)sum;
    }

    /* n >= 7 */
    seq_t pow_val = pow_uint(7, n - 7);
    seq_t result = (seq_t)279936 * pow_val;  /* 6^7 = 279936 */
    return result;
}
