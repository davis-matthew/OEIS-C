/*
 * A332775  a(n) = n + sopf(n) - omega(n).
 * https://oeis.org/A332775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + sopf(n) - omega(n) where sopf(n) is the sum of
 *            distinct prime factors of n, and omega(n) is the number of
 *            distinct prime factors
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t result = (seq_t)n;
    if (n <= 1)
        return result;

    idx_t m = n;
    seq_t sum = 0;
    idx_t count = 0;

    /* factor 2 separately */
    if ((m & 1) == 0) {
        sum += 2;
        count += 1;
        while ((m & 1) == 0)
            m >>= 1;
    }

    /* odd factors */
    for (idx_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            sum += p;
            count += 1;
            while (m % p == 0)
                m /= p;
        }
    }

    if (m > 1) {          /* remaining prime factor > sqrt(original n) */
        sum += m;
        count += 1;
    }

    result += (seq_t)sum - (seq_t)count;
    return result;
}
