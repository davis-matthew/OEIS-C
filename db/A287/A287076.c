/*
 * A287076  a(n) = least k > n with the same sum of digits as n in some base b
 *          > 1.
 * https://oeis.org/A287076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least k > n with the same sum of digits as n in some base
 *            b > 1
 * Range:     u8 n <= 15, u16 n <= 1784, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   f_base
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/f_base.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)2;
    }

    seq_t best = 0;
    bool found = false;

    for (idx_t b = 2; b <= n; ++b) {
        seq_t cand = f_base((seq_t)n, b);
        if (!found || cand < best) {
            best = cand;
            found = true;
        }
    }

    /* In the extremely unlikely event that no base b<=n yields a candidate
       (which cannot happen for n>=2), fall back to base 2. */
    if (!found)
        return f_base((seq_t)n, 2);

    return best;
}
