/*
 * A245680  Numbers x whose digits can be permuted to produce a multiple of x.
 * https://oeis.org/A245680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 8, u32 n <= 100, u64 n <= 100, u128 n <= 100,
 *            big any n
 * Helpers:   digit_counts, digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the nth positive integer whose decimal digits can be
   permuted to produce a multiple of the number (OEIS A245680). */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/pow10.h"
#include "../utils/digit_len.h"
#include "../utils/digit_counts.h"

/* Returns true if some permutation of the digits of x equals k*x for
   some integer k in [2,9].  The multiplication k*x is guaranteed to have
   the same number of decimal digits as x. */
static bool has_perm_multiple(seq_t x)
{
    int d = digit_len(x);
    seq_t lower = pow10(d - 1);
    seq_t upper = pow10(d);

    unsigned char cnt[10];
    digit_counts(x, cnt);

    for (int k = 2; k <= 9; ++k) {
        seq_t y = (seq_t)k * x;
        if (y < lower || y >= upper)
            continue;
        unsigned char cnty[10];
        digit_counts(y, cnty);
        if (memcmp(cnt, cnty, 10) == 0)
            return true;
    }
    return false;
}

/* Grows the cached table of terms to at least new_cap entries. */
static void grow_terms(size_t new_cap, seq_t **terms, size_t *cap)
{
    seq_t *new_ptr = (seq_t *)realloc(*terms, new_cap * sizeof(seq_t));
    if (!new_ptr)
        exit(1);                       /* terminate on allocation failure */
    *terms = new_ptr;
    *cap   = new_cap;
}

/* External function requested by the harness. */
seq_t a(idx_t n)
{
    static seq_t *terms   = NULL;   /* cached terms of the sequence */
    static size_t terms_cap = 0;    /* capacity of terms[]           */
    static size_t terms_len = 0;    /* number of terms stored        */
    static seq_t next_val     = 3;  /* next candidate divisible by 3 */

    /* Produce terms until we have at least n of them. */
    while (terms_len < n) {
        seq_t x = next_val;          /* candidate number, always divisible by 3 */
        if (has_perm_multiple(x)) {
            if (terms_len == terms_cap)
                grow_terms(terms_cap ? terms_cap * 2 : 4, &terms, &terms_cap);
            terms[terms_len++] = x;
        }
        next_val += 3;               /* skip numbers not divisible by 3 */
    }
    return terms[n - 1];
}
