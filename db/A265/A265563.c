/*
 * A265563  Smallest base-4 palindrome m >= n, written in base 4.
 * https://oeis.org/A265563
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   dfs_find
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find the smallest base‑4 palindrome m ≥ n (m written in base‑4).
   The function returns the decimal representation of the base‑4 palindrome
   (i.e. the digits of the base‑4 number interpreted as decimal digits).  */
#include <stdbool.h>
#include "../utils/dfs_find.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (int len = 1; ; ++len) {
        uint8_t digits[64];   /* ample space for any length needed */
        if (dfs_find(n, len, 0, digits, &result))
            return result;
    }
}
