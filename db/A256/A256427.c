/*
 * A256427  Mary Had A Little Lamb (another version).
 * https://oeis.org/A256427
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A256427: a(n) = a(n-26) for n > 26 (period 26) */
#include <stddef.h>
seq_t a(idx_t n)
{
    static const seq_t period[26] = {
        3, 2, 1, 2,
        3, 3, 3, 2,
        2, 2, 3, 5,
        5, 3, 2, 1,
        2, 3, 3, 3,
        3, 2, 2, 3,
        2, 1
    };
    idx_t i = (n - 1) % 26;
    return period[(size_t)i];
}
