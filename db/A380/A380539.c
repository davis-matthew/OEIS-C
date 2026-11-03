/*
 * A380539  The second smallest prime not dividing n.
 * https://oeis.org/A380539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the second smallest prime that does not divide n
 *            (computed by a linear search over primes)
 * Range:     u8 n <= 30030, u16 n <= 30030, u32 n <= 30030, u64 n <= 30030,
 *            u128 n <= 30030, big any n
 * Validated: standard, pass, 121 terms through n = 9678 (2026-11-01)
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_seq.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t p = 2;; ++p)
    {
        seq_t sp = (seq_t)p;
        if (!is_prime_seq(sp))
            continue;                    /* p is not prime */
        if (n % p != 0)                  /* p does not divide n */
        {
            ++count;
            if (count == 2)              /* second smallest such prime */
                return sp;
        }
    }
}
