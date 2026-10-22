/*
 * A380539  The second smallest prime not dividing n.
 * https://oeis.org/A380539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the second smallest prime that does not divide n
 *            (computed by a linear search over primes)
 * Range:     u8 n <= 9678, u16 n <= 9678, u32 n <= 9678, u64 n <= 9678,
 *            u128 n <= 9678, big any n
 * Helpers:   isPrimeTrial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/isPrimeTrial.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t p = 2;; ++p)
    {
        seq_t sp = (seq_t)p;
        if (!isPrimeTrial(sp))
            continue;                    /* p is not prime */
        if (n % p != 0)                  /* p does not divide n */
        {
            ++count;
            if (count == 2)              /* second smallest such prime */
                return sp;
        }
    }
}
