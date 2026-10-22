/*
 * A343110  Numbers having no divisor of the form 8*k + 7.
 * https://oeis.org/A343110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 147, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_no_bad_divisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers having no divisor of the form 8*k + 7.  The sequence is
   obtained by testing each positive integer in turn; a number is kept
   if no divisor d satisfies d % 8 == 7. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/has_no_bad_divisor.h"

/* Return the n-th term of the sequence (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0)   /* undefined for n==0, return 0 to keep API consistent */
        return (seq_t)0;

    idx_t count = 0;
    uint64_t cand  = 1;

    for (;;)            /* search until we hit the n-th valid number */
    {
        if (has_no_bad_divisor(cand))
        {
            ++count;
            if (count == n)
                return (seq_t)cand;
        }
        ++cand;
    }
}
