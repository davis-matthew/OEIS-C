/*
 * A319439  Number of ways to write n as the sum, u + v + w, of three base-2
 *          palindromes (from A006995) with 0 <= u <= v <= w.
 * https://oeis.org/A319439
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 280, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: for each n, pre‑compute all binary palindromes ≤ n
   and count all non‑decreasing triples (u, v, w) with u + v + w = n. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_palindrome.h"

static idx_t max_pre = 0;                 /* largest n processed so far */
static unsigned char *is_pal = NULL;      /* is_pal[x] == 1 if x is a binary palindrome */
static idx_t *pal_list = NULL;            /* sorted list of binary palindromes ≤ max_pre */
static size_t pal_count = 0;              /* number of palindromes stored */
static size_t pal_cap   = 0;              /* capacity of pal_list */

/* Ensure is_pal and pal_list contain all binary palindromes up to n. */
static void ensure_pal_until(idx_t n) {
    if (is_pal == NULL) {                 /* first initialization */
        max_pre = 0;
        is_pal = (unsigned char *)malloc(sizeof(unsigned char) * (max_pre + 1));
        if (is_pal == NULL) abort();
        is_pal[0] = 1;                    /* 0 is a palindrome */
        pal_list = (idx_t *)malloc(sizeof(idx_t) * 16);
        if (pal_list == NULL) abort();
        pal_cap  = 16;
        pal_list[0] = 0;
        pal_count = 1;
    }

    if (n <= max_pre) return;

    /* Reallocate the palindrome flag array. */
    unsigned char *new_is_pal = (unsigned char *)realloc(is_pal,
                                          (size_t)(n + 1));
    if (new_is_pal == NULL) abort();
    is_pal = new_is_pal;

    /* Ensure pal_list has enough capacity. */
    if (pal_cap < pal_count + (size_t)(n - max_pre)) {
        while (pal_cap < pal_count + (size_t)(n - max_pre))
            pal_cap *= 2;
        idx_t *tmp = (idx_t *)realloc(pal_list,
                                      sizeof(idx_t) * pal_cap);
        if (tmp == NULL) abort();
        pal_list = tmp;
    }

    /* Expand the tables up to n. */
    for (idx_t x = max_pre + 1; x <= n; ++x) {
        is_pal[x] = is_palindrome((seq_t)x, 2) ? 1 : 0;
        if (is_pal[x]) {
            pal_list[pal_count++] = x;
        }
    }
    max_pre = n;
}

seq_t a(idx_t n) {
    ensure_pal_until(n);

    seq_t count = 0;

    for (size_t ii = 0; ii < pal_count; ++ii) {
        idx_t u = pal_list[ii];
        if (u > n) break;
        for (size_t jj = ii; jj < pal_count; ++jj) {
            idx_t v = pal_list[jj];
            idx_t sum_uv = u + v;
            if (sum_uv > n) break;
            idx_t w = n - sum_uv;
            if (w < v) break;               /* further v will only make w smaller */
            if (is_pal[w]) count += 1;
        }
    }

    return count;
}
