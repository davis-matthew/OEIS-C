/*
 * A261921  Nonpalindromes which are the sum of two palindromes whose lengths
 *          differ by 1.
 * https://oeis.org/A261921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A261921: nonpalindromes which are the sum of two palindromes
   whose lengths differ by 1.  The function searches consecutively for
   the next term each time it is requested. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_palindrome.h"   /* provides:  bool is_palindrome(seq_t x, idx_t base); */

/* Return the number of decimal digits of x (x>0). */
static inline idx_t digit_len_seq(seq_t x)
{
    idx_t len = 0;
    while (x > 0) {
        x /= 10;
        ++len;
    }
    return len;
}

seq_t a(idx_t n)
{
    /* Cache of previously found sequence terms. */
    static seq_t *seq_cache = NULL;
    static size_t seq_cap  = 0;
    static idx_t seq_len   = 0;   /* number of stored terms */

    /* Cache of palindromes up to current limit. */
    static seq_t *pal_cache = NULL;
    static size_t pal_cap   = 0;
    static idx_t pal_len    = 0;   /* number of stored palindromes */
    static idx_t max_pal_pre = 0;  /* largest number already examined for palindromes */

    /* Next integer to test for membership in the sequence. */
    static idx_t next_candidate = 1;

    /* Extend palindrome cache to cover up to next_candidate. */
    if (max_pal_pre < next_candidate) {
        for (idx_t i = max_pal_pre + 1; i <= next_candidate; ++i) {
            if (is_palindrome((seq_t)i, (idx_t)10)) {
                if (pal_len >= pal_cap) {
                    pal_cap = pal_cap ? pal_cap * 2 : 4;
                    pal_cache = (seq_t *)realloc(pal_cache, pal_cap * sizeof(seq_t));
                }
                pal_cache[pal_len++] = (seq_t)i;
            }
        }
        max_pal_pre = next_candidate;
    }

    /* Generate terms until we reach the requested index. */
    while (seq_len < n) {
        /* Consider all representations of next_candidate as sum of two palindromes. */
        bool ok = false;
        for (size_t i = 0; i < pal_len && pal_cache[i] < next_candidate; ++i) {
            seq_t p = pal_cache[i];
            seq_t q = next_candidate - p;
            if (is_palindrome(q, (idx_t)10)) {
                idx_t d1 = digit_len_seq(p);
                idx_t d2 = digit_len_seq(q);
                if ((d1 > d2 ? d1 - d2 : d2 - d1) == 1) {
                    ok = true;
                    break;
                }
            }
        }
        if (ok) {
            if (seq_len >= seq_cap) {
                seq_cap = seq_cap ? seq_cap * 2 : 4;
                seq_cache = (seq_t *)realloc(seq_cache, seq_cap * sizeof(seq_t));
            }
            seq_cache[seq_len++] = next_candidate;
        }
        ++next_candidate;
    }

    return seq_cache[n - 1];
}
