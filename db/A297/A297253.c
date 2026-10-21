/*
 * A297253  Numbers whose base-4 digits having equal up-variation and
 *          down-variation; see Comments.
 * https://oeis.org/A297253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   equal_variation
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A297253 by brute‑force search over natural
   numbers, checking for equal up‑variation and down‑variation in base‑4. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/equal_variation.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* n is 1‑based. */
    seq_t target = (seq_t)n;   /* cast once for comparison */
    seq_t count  = 0;
    seq_t value  = 1;          /* start from the smallest positive integer */

    while (true)
    {
        if (equal_variation(value))
        {
            count++;
            if (count == target)
                return value;
        }
        value++;
    }
}
