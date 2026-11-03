/*
 * A279421  Numbers k such that k^2 has an odd number of digits and the middle
 *          digit is 1.
 * https://oeis.org/A279421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   numbers k such that k^2 has an odd number of decimal digits and
 *            the middle digit of k^2 is 1
 * Range:     u8 n <= 1, u16 n <= 14, u32 n <= 2499, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 63 terms through n = 9449 (2026-11-01)
 * Helpers:   ipow_idx, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow_idx.h"
#include "../utils/num_digits.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t k = 0;

    for (;;) {
        k += 1;                               /* candidate k */

        seq_t sq = ipow_idx(k, 2);              /* k² */
        int L   = num_digits(sq);              /* number of decimal digits of k² */

        if ((L & 1) == 0)                     /* even number of digits → skip */
            continue;

        seq_t pow10_m = ipow_idx(10, L / 2);   /* 10^(L/2) */
        seq_t mid     = (sq / pow10_m) % 10;   /* middle digit */

        if (mid == 1) {                       /* found a valid k */
            count++;
            if (count == n)
                return k;                     /* nth term */
        }
    }
}
