/*
 * A261189  Integers such that no subsequence of decimal representation is
 *          divisible by 5.
 * https://oeis.org/A261189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 168, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _contains_invalid_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Method: brute‑force search for the n-th positive integer whose decimal
 * representation contains no digits 0 or 5. */

#include <stdbool.h>
#include "../utils/_contains_invalid_digit.h"

/* Return the n-th such integer (1‑based indexing). */
seq_t a(idx_t n)
{
    idx_t   cnt = 0;          /* how many valid numbers seen so far */
    seq_t   cur = 1;          /* candidate number */

    while (1)
    {
        if (!_contains_invalid_digit(cur))
        {
            cnt++;
            if (cnt == n)
                return cur;   /* found the n-th valid integer */
        }
        cur++;                /* next candidate */
    }
}
