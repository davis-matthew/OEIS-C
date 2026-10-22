/*
 * A346221  Palindromes that are multiples of 11 and whose digit sum is also a
 *          multiple of 11.
 * https://oeis.org/A346221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 13, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   digit_sum, reverse_decimal, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: find palindromes that are multiples of 11 and whose digit
 * sum is also a multiple of 11.  The function generates palindromes
 * in increasing order, tests the two divisibility conditions, and
 * stores the qualifying numbers in a static cache so that repeated
 * calls are O(1). */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/seq_pow.h"
#include "../utils/reverse_decimal.h"
#include "../utils/digit_sum.h"

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                    /* not defined for n < 1 */

    /* --------- cache for found terms -------------------------------- */
    static seq_t *cache   = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_len  = 0;

    if (!cache) {                          /* one‑time allocation      */
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            exit(EXIT_FAILURE);
    }

    /* --------- state for palindrome generation --------------------- */
    static idx_t cur_len  = 1;          /* current palindrome length    */
    static seq_t cur_start = 1;         /* first left half for cur_len  */
    static seq_t cur_end   = 9;         /* last  left half for cur_len  */
    static seq_t cur_left  = 1;         /* next left half to use        */

    /* --------- produce terms until we have the n‑th one ------------- */
    while (cache_len < n) {
        size_t half = (cur_len % 2 == 0) ?
                        (size_t)(cur_len / 2) :
                        (size_t)((cur_len - 1) / 2);

        seq_t mult = seq_pow((seq_t)10, (idx_t)half);
        seq_t rev  = (cur_len % 2 == 0) ?
                        reverse_decimal(cur_left) :
                        reverse_decimal(cur_left / (seq_t)10);

        seq_t pal = cur_left * mult + rev;

        if ((pal % (seq_t)11 == 0) &&
            (digit_sum(pal) % (seq_t)11 == 0)) {
            /* enlarge cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap * 2 + 64;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) { free(cache); exit(EXIT_FAILURE); }
                cache   = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = pal;
        }

        /* ---- move to next candidate --------------------------------- */
        if (cur_left == cur_end) {
            /* advance to next palindrome length */
            ++cur_len;
            idx_t left_len = (cur_len + 1) / 2;
            cur_start = seq_pow((seq_t)10, (idx_t)(left_len - 1));
            cur_end   = seq_pow((seq_t)10, (idx_t)left_len) - (seq_t)1;
            cur_left  = cur_start;
        } else {
            ++cur_left;
        }
    }

    return cache[n - 1];
}
