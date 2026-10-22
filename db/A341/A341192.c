/*
 * A341192  Number of ways to write n as an ordered sum of 3 nonzero decimal
 *          palindromes.
 * https://oeis.org/A341192
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 433, u16 n <= 9323, u32 n <= 9323, u64 n <= 9323,
 *            u128 n <= 9323, big any n
 * Helpers:   is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of ordered triples of nonzero decimal palindromes summing to n */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_palindrome.h"

static idx_t max_n_cached = 0;
static bool *pal_bool = NULL;
static size_t pal_bool_cap = 0;
static idx_t *pal_list = NULL;
static size_t pal_list_cap = 0;
static size_t pal_count = 0;

/* Ensure pal_bool[0..limit] and pal_list contain all palindromes <= limit. */
static void ensure_pal_up_to(idx_t limit)
{
    if (limit <= max_n_cached)
        return;

    size_t new_cap = (size_t)(limit + 1);

    /* Resize boolean array if needed */
    if (pal_bool_cap < new_cap) {
        bool *tmp = (bool *)realloc(pal_bool, new_cap * sizeof(bool));
        if (!tmp) abort();
        /* Initialize new entries to false */
        for (size_t i = pal_bool_cap; i < new_cap; ++i)
            tmp[i] = false;
        pal_bool = tmp;
        pal_bool_cap = new_cap;
    }

    /* Resize palindrome list array if needed */
    if (pal_list_cap < pal_count + 1) {
        size_t new_cap_list = pal_list_cap ? pal_list_cap * 2 : 8;
        idx_t *tmp = (idx_t *)realloc(pal_list, new_cap_list * sizeof(idx_t));
        if (!tmp) abort();
        pal_list = tmp;
        pal_list_cap = new_cap_list;
    }

    /* Compute palindromes from max_n_cached+1 up to limit */
    for (idx_t i = max_n_cached + 1; i <= limit; ++i) {
        bool ispal = is_palindrome((seq_t)i, 10);
        pal_bool[(size_t)i] = ispal;
        if (ispal) {
            if (pal_count == pal_list_cap) {
                size_t new_cap_list = pal_list_cap ? pal_list_cap * 2 : 8;
                idx_t *tmp = (idx_t *)realloc(pal_list, new_cap_list * sizeof(idx_t));
                if (!tmp) abort();
                pal_list = tmp;
                pal_list_cap = new_cap_list;
            }
            pal_list[pal_count++] = i;
        }
    }

    max_n_cached = limit;
}

seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)0;

    ensure_pal_up_to(n);

    seq_t count = 0;
    for (size_t i = 0; i < pal_count; ++i) {
        idx_t p = pal_list[i];
        if (p > n)
            break;
        for (size_t j = 0; j < pal_count; ++j) {
            idx_t q = pal_list[j];
            if (q > n)
                break;
            idx_t sum = p + q;
            if (sum > n)
                continue;
            idx_t r = n - sum;
            if (r == 0)
                continue;
            /* r <= n by construction */
            if (pal_bool[(size_t)r])
                ++count;
        }
    }
    return count;
}
