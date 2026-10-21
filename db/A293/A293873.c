/*
 * A293873  Numbers having '13' as substring of their digits.
 * https://oeis.org/A293873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the nth integer whose decimal representation contains the
 *            substring "13"
 * Range:     u8 n <= 13, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   contains13
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/contains13.h"

/* Return the nth term of the sequence. n is 1‑based. */
seq_t a(idx_t n)
{
    if (n == 0u)
        return 0u;                       /* undefined, but safe */

    idx_t found = 0u;
    seq_t num = 13u;                    /* first candidate containing "13" */

    while (found < n)
    {
        if (contains13(num))
        {
            ++found;
            if (found == n)
                return num;
        }
        ++num;
    }
    return 0u;                           /* unreachable, keeps function total */
}
