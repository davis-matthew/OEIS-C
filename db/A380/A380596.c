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
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-01)
 * Helpers:   none
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

/* Determine whether x has an embedded palindrome of length 2 or 3.
 * The digits are extracted least‑significant first, but the tests
 * are performed in that order.  For a 3‑digit ded that would be the
 * whole number (e.g. 101) we do *not* count it as an embedded
 * palindrome, because the palindrome must be a proper substring. */
static bool has_embedded_palindrome(seq_t x)
{
    /* 3 × 10^9 fits into 32 bits; we only need at most 24 decimal
     * digits for the range of interest.  Use 32 to be safe. */
    int d[32];
    int len = 0;
    seq_t t = x;
    do {
        d[len++] = (int)(t % (seq_t)10);
        t /= (seq_t)10;
    } while (t);

    for (int i = 0; i + 1 < len; ++i) {
        if (d[i] == d[i + 1])          /* dd */
            return true;
        if (i + 2 < len) {
            /* If the number has exactly 3 digits and i == 0, the ded
             * would be the whole number; skip that case. */
            if (!(len == 3 && i == 0) && d[i] == d[i + 2])  /* ded */
                return true;
        }
    }
    return false;
}

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
