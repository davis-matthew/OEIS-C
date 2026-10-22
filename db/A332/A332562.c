/*
 * A332562  a(n) = number formed by concatenating the decimal digits of 44,
 *          45, 46, ..., 44+n.
 * https://oeis.org/A332562
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concat(44, 45, …, 44+n)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 8, u128 n <= 18,
 *            big any n
 * Helpers:   digit_count, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/digit_count.h"

/* Compute a(n) for OEIS sequence A332562.
 *
 * Parameters:
 *   n  - index, 0-based.
 *
 * Returns:
 *   The integer formed by concatenating the decimal digits of
 *   44, 45, …, 44+n.
 */
seq_t a(idx_t n)
{
    const idx_t start = 44;
    const idx_t end   = 44 + n;          /* inclusive */
    seq_t result = 0;                    /* start with zero */

    for (idx_t i = start; i <= end; ++i) {
        /* Number of decimal digits of i.  digit_count returns a
           uint64_t; this is safe because i fits in uint64_t. */
        uint64_t digits = digit_count(i);

        /* 10^digits as a seq_t */
        seq_t pow10_digits = pow10(digits);

        /* Shift the current result digits left and add i. */
        result = result * pow10_digits + (seq_t)i;
    }

    return result;
}
