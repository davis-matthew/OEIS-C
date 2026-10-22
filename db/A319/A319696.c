/*
 * A319696  Number of distinct values obtained when Euler phi (A000010) is
 *          applied to the divisors of n.
 * https://oeis.org/A319696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct values of Euler’s totient applied to
 *            the divisors of n
 * Range:     u8 n <= 1941, u16 n <= 9988, u32 n <= 9988, u64 n <= 9988,
 *            u128 n <= 9988, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* The number of distinct φ‐values for n ≤ 10 000 is far below 128;
       the array is more than large enough. */
    seq_t distinct[128];
    size_t count = 0;

    for (idx_t d = 1; d <= n / d; ++d)
    {
        if (n % d == 0)
        {
            seq_t val = phi(d);
            bool unique = true;
            for (size_t i = 0; i < count; ++i)
                if (val == distinct[i]) { unique = false; break; }
            if (unique)
                distinct[count++] = val;

            idx_t e = n / d;
            if (e != d)
            {
                val = phi(e);
                unique = true;
                for (size_t i = 0; i < count; ++i)
                    if (val == distinct[i]) { unique = false; break; }
                if (unique)
                    distinct[count++] = val;
            }
        }
    }

    return (seq_t)count;
}
