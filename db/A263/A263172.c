/*
 * A263172  Nonpalindromic positive integers whose digits can be rearranged to
 *          form a palindrome, after evaluation.
 * https://oeis.org/A263172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 7099, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_palindrome_impl, odd_nonzero_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A263172 – Dennis numbers
 *
 *  Sequence definition:
 *    * The number is not itself a palindrome (in base‑10).
 *    * Its decimal digits can be permuted to form a palindrome
 *      after dropping any leading zeros of the permutation.
 *      (Equivalently: the non‑zero digits contain at most one digit
 *      with an odd multiplicity; the zeroes may have any multiplicity
 *      and can be omitted from the front.)
 *
 *  The implementation searches for Dennis numbers in increasing order.
 *  The sequence is memoised in a static array so that subsequent calls
 *  to `a()` for any index `n >= 1` are fast.
 *
 *  All operations use only integer arithmetic supported by `seq_t`.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_palindrome_impl.h"
#include "../utils/odd_nonzero_digits.h"

/* ---------- Local helper functions ----------------------------------- */

/* ---------- Sequence generator --------------------------------------- */
seq_t a(idx_t n)
{
    /* guard against requests before the offset – return 0 for n == 0 */
    if (n == (idx_t)0) return (seq_t)0;

    /* static cache for memoisation */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;   /* number of terms stored */
    static size_t cache_cap  = 0;   /* current capacity of cache */
    static seq_t last_checked = (seq_t)9;   /* start before 10 */

    /* grow the cache until it contains at least n terms */
    while (cache_len < (size_t)n) {
        seq_t cand = last_checked + (seq_t)1;   /* next candidate */

        /* not a palindrome itself */
        if (!is_palindrome_impl(cand)) {
            /* does it satisfy the digit condition? */
            if (odd_nonzero_digits(cand) <= 1U) {
                /* store the term */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2U : 16U;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (tmp == NULL) {
                        /* allocation failure – abort with zero */
                        return (seq_t)0;
                    }
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = cand;
            }
        }

        last_checked = cand;   /* advance search */
    }

    return cache[(size_t)n - 1U];
}
