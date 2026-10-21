/*
 * A280825  Numbers with an odd number of digits and with an even number of
 *          distinct digits.
 * https://oeis.org/A280825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   digit_counts, digit_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: brute‑force search for the n‑th integer with an odd number of decimal digits and an even number of distinct decimal digits. */

#include <stdbool.h>
#include "../utils/digit_len.h"
#include "../utils/digit_counts.h"

/* Return true if x has an odd number of digits and an even number of distinct digits. */
static bool satisfies(seq_t x)
{
    int len = digit_len(x);               /* number of decimal digits */
    if ((len & 1) == 0) return false;     /* need odd length */

    unsigned char cnt[10] = {0};
    digit_counts(x, cnt);                /* counts of each digit */

    int distinct = 0;
    for (int i = 0; i < 10; ++i)
        if (cnt[i]) ++distinct;

    return (distinct & 1) == 0;           /* even number of distinct digits */
}

/* Main function required by the framework. */
seq_t a(idx_t n)
{
    idx_t   found = 0;
    seq_t   val   = 0;

    for (;;) {
        ++val;                            /* next candidate */

        if (satisfies(val)) {
            ++found;
            if (found == n)
                return val;
        }
    }
}
