/*
 * A391273  Smallest base b >= 2 for which one "Reverse and Add" step in base
 *          b starting from n yields a palindrome in base b.
 * https://oeis.org/A391273
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 9638, u32 n <= 9638, u64 n <= 9638,
 *            u128 n <= 9638, big any n
 * Helpers:   is_palindrome, reverse_in_base
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Smallest base b ≥ 2 for which one "Reverse and Add" step in base b starting from n
   yields a palindrome in base b.  By definition a(n) always exists and is ≤ 2·n−1. */

#include <stdbool.h>
#include "../utils/is_palindrome.h"
#include "../utils/reverse_in_base.h"

/* External linkage as required. */
seq_t a(idx_t n)
{
    if (n < 2)                /* undefined below offset, but safe to return 0 */
        return (seq_t)0;

    const seq_t n_seq = (seq_t)n;
    const idx_t   limit = 2 * n - 1;   /* guaranteed upper bound for a(n) */

    for (idx_t b = 2; b <= limit; ++b)
    {
        const seq_t rev  = reverse_in_base(n_seq, (seq_t)b);
        const seq_t sum  = n_seq + rev;
        if (is_palindrome(sum, b))
            return (seq_t)b;
    }

    /* This point should never be reached because of the bound, but return
       the upper limit to keep the function total. */
    return (seq_t)limit;
}
