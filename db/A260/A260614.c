/*
 * A260614  Product of the first n ultrafactorials ((n!)^(n!)).
 * https://oeis.org/A260614
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏_{k=1..n} (k!)^(k!)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Validated: standard, pass, 6 terms through n = 5 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    seq_t result   = 1;
    seq_t factorial = 1;

    for (idx_t k = 1; k <= n; ++k) {
        factorial *= (seq_t)k;               /* factorial = k!           */
        seq_t power = ipow(factorial, factorial); /* (k!)^(k!) */
        result *= power;                     /* accumulate product       */
    }

    return result;
}
