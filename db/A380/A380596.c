/*
 * A380596  Numbers with embedded palindromes as proper substrings of the
 *          term.
 * https://oeis.org/A380596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   has_embedded_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A search-based implementation: a(n) is the n‑th integer ≥ 100 that
 * contains a proper substring of the form dd or ded (two or three
 * contiguous digits that form a palindrome).  The function
 * iterates over candidates until it finds the nth one.  This satisfies
 * OEIS A380596 */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/has_embedded_palindrome.h"

seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t cand = (seq_t)100;   /* start from the first admissible number */

    while (1) {
        if (has_embedded_palindrome(cand)) {
            ++found;
            if (found == n)
                return cand;
        }
        ++cand;
    }
}
