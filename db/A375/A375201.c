/*
 * A375201  a(n) is the sum of the bases b with 1 < b < n-1 in which n is a
 *          palindrome.
 * https://oeis.org/A375201
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = sum of bases b with 1 < b < n-1 in which n is a
 *            palindrome
 * Range:     u8 n <= 167, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We must include the helper that actually checks for palindromes in a given base. */
#include <stdbool.h>
#include "../utils/is_palindrome.h"

seq_t a(idx_t n)
{
    /* The offset is 4: a(4) is the first defined value. For n < 4 the value is
       irrelevant and we simply return 0. */
    if (n < 4) return 0;

    seq_t sum = 0;
    /* Consider bases b from 2 up to n-2 (inclusive). Recall that b < n-1. */
    for (idx_t b = 2; b < n - 1; ++b) {
        /* is_palindrome returns true if n (cast to seq_t) is a palindrome in
           base b. If so, add the base to the running sum. */
        if (is_palindrome((seq_t)n, b))
            sum += (seq_t)b;
    }
    return sum;
}
