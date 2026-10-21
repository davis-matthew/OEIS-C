/*
 * A265535  Largest base-6 palindrome m <= n, written in base 10.
 * https://oeis.org/A265535
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the largest base‑6 palindrome ≤ n (result in base‑10).
   It evaluates candidates in descending order until a palindrome
   is found, using the provided is_palindrome function. */
#include <stdbool.h>
#include "../utils/is_palindrome.h"

seq_t a(idx_t n)
{
    /* First test whether n itself is a palindrome */
    if (is_palindrome((seq_t)n, 6))
        return (seq_t)n;

    /* Scan downwards from n−1 until a palindrome is found. */
    for (idx_t i = n; ; --i)
    {
        if (is_palindrome((seq_t)i, 6))
            return (seq_t)i;
        /* Since idx_t is unsigned, i will wrap around after reaching 0.
           Break explicitly to avoid an infinite loop. */
        if (i == 0)
            break;
    }

    /* For n == 0 the above loop returns 0, so this line is unreachable,
       but is kept for completeness. */
    return (seq_t)0;
}
