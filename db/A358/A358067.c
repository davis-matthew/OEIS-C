/*
 * A358067  a(n) is the smallest m such that A144261(m) = n.
 * https://oeis.org/A358067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the smallest positive integer m such that A144261(m) =
 *            n, where A144261(m) is defined as the smallest k with m*k a
 *            Niven number. Equivalently, m*n is a Niven number and for every
 *            1 ≤ t < n, m*t is not a Niven number. This implementation
 *            searches for m incrementally, skipping any m that is...
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/digit_sum.h"

static inline bool is_harshad(seq_t x)
{
    seq_t d = digit_sum(x);
    if (d == 0)
        return false;
    return (x % d) == 0;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return 1;               /* a(1) = 1 by definition */

    seq_t nseq = (seq_t)n;
    seq_t m = 1;

    for (;; ++m)
    {
        /* m*1 must not be Niven: skip compositions where m is Niven */
        if (is_harshad(m))
            continue;

        seq_t prod = m * nseq;
        if (!is_harshad(prod))
            continue;           /* n*m must be Niven */

        /* Verify that no smaller multiplier yields a Niven number */
        bool ok = true;
        for (idx_t t = 2; t < n; ++t)     /* t = 1 already checked */
        {
            if (is_harshad(m * (seq_t)t))
            {
                ok = false;
                break;
            }
        }
        if (ok)
            return m;
    }
}
