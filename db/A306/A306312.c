/*
 * A306312  Number of terms of the set of divisors of n that are not the
 *          product of any other two distinct divisors.
 * https://oeis.org/A306312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Σ_{p^e‖n} min(2, e) The sum runs over all prime
 *            powers dividing n. This counts 1, all primes dividing n, and the
 *            squares of primes dividing n, which are exactly the divisors not
 *            expressible as a product of two distinct >1 divisors
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)           /* out of range of OEIS offset, but safe */
        return 0;

    seq_t count = 1;      /* always include divisor 1 */
    idx_t temp = n;

    /* factor 2 separately */
    if (temp % 2 == 0) {
        idx_t e = 0;
        while (temp % 2 == 0) {
            temp /= 2;
            ++e;
        }
        /* add contributions of exponent 1 and 2 */
        count += (e >= 1 ? 1 : 0);
        count += (e >= 2 ? 1 : 0);
    }

    /* odd factors */
    for (idx_t i = 3; i <= temp / i; i += 2) {
        if (temp % i == 0) {
            idx_t e = 0;
            while (temp % i == 0) {
                temp /= i;
                ++e;
            }
            count += (e >= 1 ? 1 : 0);
            count += (e >= 2 ? 1 : 0);
        }
    }

    /* remaining prime factor > 1 */
    if (temp > 1)
        count += 1;          /* exponent 1 */

    return count;
}
