/*
 * A391793  Multiplicative sequence a(n) with a(p^e) = ((e mod 5) * ((e mod 5)
 *          - 5) + 4) / 2 for prime p and e > 0.
 * https://oeis.org/A391793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = multiplicative; for prime p, a(p^e) = ((e mod 5) * ((e
 *            mod 5) - 5) + 4) / 2
 * Range:     i8 n <= 100, i16 n <= 100, i32 n <= 100, i64 n <= 100,
 *            i128 n <= 100, big any n
 * Helpers:   per_exp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/per_exp.h"

/* multiplicative sequence A391793 */
seq_t a(idx_t n)
{
    if (n <= 1)                     /* offset is 1 */
        return (n == 1 ? (seq_t)1 : (seq_t)0);

    seq_t result = (seq_t)1;
    idx_t m = n;
    idx_t e = 0;

    /* factor out the power of 2 first */
    while ((m & 1) == 0)
    {
        m >>= 1;
        ++e;
    }
    if (e > 0)
        result *= per_exp(e);

    /* factor odd primes */
    for (idx_t i = 3; i <= m / i; i += 2)
    {
        if (m % i == 0)
        {
            e = 0;
            while (m % i == 0)
            {
                m /= i;
                ++e;
            }
            result *= per_exp(e);
        }
    }

    /* remaining prime factor > 1 */
    if (m > 1)
        result *= per_exp(1);

    return result;
}
