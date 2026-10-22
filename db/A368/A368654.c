/*
 * A368654  Decimal expansion of 158452/21444.
 * https://oeis.org/A368654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = the nth decimal digit of 158452/21444 (offset 1)
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   alloc_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/alloc_cache.h"

seq_t a(idx_t n)
{
    /* reduced fraction 158452/21444 = 39613/5361 */
    static const idx_t NUM     = 39613;
    static const idx_t DEN     = 5361;
    static const idx_t INT_PART = NUM / DEN;              /* integer part (7) */

    static seq_t *digits = NULL;   /* array of fractional digits */
    static idx_t  period  = 0;     /* length of repeating block */
    static int    init    = 0;     /* one‑time initialisation flag */

    if (!init) {
        init = 1;

        /* Allocate space for at most DEN digits (worst case). */
        digits = alloc_cache(DEN);

        idx_t rem = NUM % DEN;
        idx_t r0  = rem;   /* first remainder after integer part */

        idx_t i = 0;
        while (1) {
            idx_t t = rem * 10;                     /* shifted remainder  */
            digits[i] = (seq_t)(t / DEN);           /* next decimal digit   */
            rem = t % DEN;                          /* new remainder        */
            i++;

            if (rem == r0 || rem == 0) {            /* end of period       */
                period = i;
                break;
            }
        }
    }

    if (n == 1)
        return (seq_t)INT_PART;                        /* integer digit */

    /* n >= 2 : return the (n-2)th fractional digit, cycling with period */
    idx_t pos = (n - 2) % period;
    return digits[(size_t)pos];
}
