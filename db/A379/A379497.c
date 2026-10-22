/*
 * A379497  Dirichlet inverse of A046897, where A046897 is the sum of divisors
 *          of n that are not divisible by 4.
 * https://oeis.org/A379497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   multiplicative Dirichlet inverse of A046897. a(2^e) = -3 *
 *            (-2)^(e-1) a(p) = -(p+1) (odd prime) a(p^2) = p a(p^e) = 0 for e
 *            ≥ 3 (odd prime) a(1) = 1
 * Range:     i8 n <= 59, i16 n <= 9915, i32 n <= 9915, i64 n <= 9915,
 *            i128 n <= 9915, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    idx_t tmp = n;

    /* handle factor 2 separately */
    idx_t cnt2 = 0;
    while ((tmp & 1) == 0) {   /* tmp % 2 == 0 */
        cnt2++;
        tmp >>= 1;              /* tmp /= 2 */
    }
    if (cnt2 > 0) {
        /* a(2^cnt2) = -3 * (-2)^(cnt2-1) */
        seq_t pow = (seq_t)1;
        for (idx_t k = 1; k < cnt2; ++k)
            pow *= (seq_t)-2;
        seq_t val2 = (seq_t)-3 * pow;
        result *= val2;
    }

    /* factor remaining odd part */
    idx_t x = tmp;
    for (idx_t p = 3; p * p <= x; p += 2) {
        if (x % p == 0) {
            idx_t exp = 0;
            while (x % p == 0) {
                x /= p;
                exp++;
            }
            seq_t val;
            if (exp == 1)
                val = -( (seq_t)p + (seq_t)1 );
            else if (exp == 2)
                val = (seq_t)p;
            else
                return (seq_t)0;   /* prime power exponent ≥ 3 gives 0 */
            result *= val;
            if (x == 1)
                break;
        }
    }

    if (x > 1) { /* remaining prime factor (exponent 1) */
        seq_t val = -( (seq_t)x + (seq_t)1 );
        result *= val;
    }

    return result;
}
