/*
 * A245853  Powers of 12 without the digit '0' in their decimal expansion.
 * https://oeis.org/A245853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 9, u128 n <= 11,
 *            big any n
 * Helpers:   has_no_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the n-th power of 12 whose decimal expansion contains no digit 0.
 * The code computes powers of 12 one by one and counts those that satisfy the
 * zero‑digit test until the n‑th such power is found.  No dynamic allocation
 * is required, so the implementation is valid both for plain integral
 * seq_t types and for arbitrary‑precision integer classes.
 */
#include <stdbool.h>    /* for bool */
#include "../utils/has_no_zero.h"

/* a(n) – 1‑based index, a(1) = 1 */
seq_t a(idx_t n)
{
    seq_t power = 1;      /* 12^0 */
    idx_t   count = 1;    /* first term found */

    if (n == 1)
        return power;

    for (;;) {
        power *= (seq_t)12;
        if (has_no_zero(power)) {
            count++;
            if (count == n)
                return power;
        }
    }
}
