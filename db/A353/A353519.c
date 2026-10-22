/*
 * A353519  a(n) = 1 if n has an odd number of square divisors, otherwise 0.
 * https://oeis.org/A353519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 1 if every prime exponent e in n satisfies e mod 4 ∈
 *            {0,1}; otherwise 0. Equivalent to: n has an odd number of square
 *            divisors
 * Range:     u8 n <= 9590, u16 n <= 9590, u32 n <= 9590, u64 n <= 9590,
 *            u128 n <= 9590, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1)          /* below the OEIS offset – any value is acceptable */
        return (seq_t)0;

    idx_t temp = n;

    /* factor 2 separately */
    if ((temp & 1) == 0) {
        int e = 0;
        while ((temp & 1) == 0) {
            temp >>= 1;
            e++;
        }
        if (e % 4 >= 2)   /* e mod 4 = 2 or 3 */
            return (seq_t)0;
    }

    /* factor odd primes */
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                e++;
            }
            if (e % 4 >= 2)
                return (seq_t)0;
        }
    }

    /* if temp > 1 it is a prime factor of exponent 1, which is acceptable */
    return (seq_t)1;
}
