/*
 * A260855  Base-5 representation of a(n) is the concatenation of the base-5
 *          representations of 1, 2, ..., n, n-1, ..., 1.
 * https://oeis.org/A260855
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the integer whose base‑5 representation is the
 *            concatenation of the base‑5 representations of 1,2,…,n, n‑1,…,1.
 *            This function builds that integer by repeatedly appending the
 *            base‑5 digits of each number
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   digit_len_base5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/digit_len_base5.h"

/* Compute a(n) for A260855. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* empty list */

    seq_t result = 0;

    /* Append 1 .. n */
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t x = i;
        unsigned len = digit_len_base5(i);
        seq_t mult = 1;
        for (unsigned k = 0; k < len; ++k)
            mult *= 5;               /* 5^len */
        result = result * mult + x;
    }

    /* Append n-1 .. 1 (descending). */
    for (idx_t i = n - 1; i > 0; --i)
    {
        seq_t x = i;
        unsigned len = digit_len_base5(i);
        seq_t mult = 1;
        for (unsigned k = 0; k < len; ++k)
            mult *= 5;               /* 5^len */
        result = result * mult + x;
    }

    return result;
}
