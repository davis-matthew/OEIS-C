/*
 * A358975  Numbers that are coprime to their digital sum in base 3 (A053735).
 * https://oeis.org/A358975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 95, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k such that gcd(k, sum of ternary digits of k) = 1.
   Computed by incremental scanning from 1 (no caching). */
#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;        /* undefined for n < 1 */

    idx_t count = 0;
    seq_t k = 1;

    while (count < n) {
        /* compute sum of base‑3 digits of k */
        seq_t temp = k;
        seq_t sum  = 0;
        while (temp > 0) {
            sum += temp % 3;
            temp /= 3;
        }

        if (gcd(k, sum) == (seq_t)1) {
            ++count;
            if (count == n)
                return k;
        }
        ++k;
    }

    return (seq_t)0; /* unreachable */
}
