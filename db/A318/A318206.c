/*
 * A318206  Numbers having no divisor d > 1 that is a binary palindrome (i.e.,
 *          an element of A006995).
 * https://oeis.org/A318206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 101, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers having no divisor d > 1 that is a binary palindrome (A318206).
   The function builds the sequence on demand, storing already found terms
   in a static cache.  A candidate integer is accepted iff none of its
   divisors > 1, including itself, is a binary palindrome.  Divisors are
   tested by trial division up to sqrt(n).  The helper function
   `is_palindrome` (from the utils) is used to test binary palindromes.  */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_palindrome.h"

/* Cache of sequence values.  */
static seq_t *seq_cache = NULL;
static size_t  seq_len  = 0;
static size_t  seq_cap  = 0;

/* Next integer to be examined when filling the cache.  */
static uint64_t next_candidate = 2;

/* Grow the cache array to accommodate at least `need` elements.  */
static void grow_cache(size_t need)
{
    if (seq_cap >= need) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 8;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    seq_cache = tmp;
    seq_cap   = new_cap;
}

/* Append a new term to the cache.  */
static void add_term(seq_t val)
{
    grow_cache(seq_len + 1);
    seq_cache[seq_len++] = val;
}

/* Test whether `n` has any divisor d > 1 that is a binary palindrome.
   `n` is assumed to fit in uint64_t.  */
static bool has_binpal_divisor(uint64_t n)
{
    if (n <= 1) return false;

    /* Check all divisors up to sqrt(n).  */
    for (uint64_t d = 2; d * d <= n; ++d) {
        if (n % d == 0) {
            if (is_palindrome((seq_t)d, 2)) return true;
            uint64_t q = n / d;
            if (is_palindrome((seq_t)q, 2)) return true;
        }
    }
    /* Finally, test `n` itself.  */
    return is_palindrome((seq_t)n, 2);
}

/* External function required by the framework.  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* Offset is 1; undefined for 0. */

    /* Ensure the first term is present.  */
    if (seq_len == 0) add_term((seq_t)1);

    /* Generate terms until the cache holds at least `n` elements.  */
    while (seq_len < (size_t)n) {
        uint64_t cand = next_candidate++;
        if (!has_binpal_divisor(cand)) {
            add_term((seq_t)cand);
        }
    }

    return seq_cache[(size_t)n - 1];
}
