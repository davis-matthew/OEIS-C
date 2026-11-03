/*
 * A319387  Smallest palindrome p such that n-p is again a palindrome, or n if
 *          no such p exists.
 * https://oeis.org/A319387
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 1029, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 8793 (2026-11-01)
 * Helpers:   is_palindrome_base
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: For a given n, find the smallest palindrome p (base 10)
   such that n – p is also a palindrome.  If no such p exists, return n. */

#include <stdbool.h>
#include "../utils/is_palindrome_base.h"

seq_t a(idx_t n) {
    /* Convert index to seq_t once for efficient arithmetic. */
    seq_t n_seq = (seq_t)n;

    /* Search palindromes p from 0 up to n. */
    for (idx_t p = 0; p <= n; ++p) {
        seq_t p_seq = (seq_t)p;
        /* Check if both p and n-p are palindromes. */
        if (is_palindrome_base(p_seq, 10) && is_palindrome_base(n_seq - p_seq, 10))
            return p_seq;   /* Found the smallest suitable p. */
    }

    /* No palindrome pair found; return n itself. */
    return n_seq;
}
