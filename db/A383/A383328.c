/*
 * A383328  Numbers that have the same set of digits as the sum of the squares
 *          of their digits.
 * https://oeis.org/A383328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A383328: a(n) is a number that has the same set of decimal digits
    as the sum of the squares of its decimal digits.
    The routine generates terms in increasing order and caches them
    so that calls with arbitrary n are answered correctly. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/digit_counts.h"

/* cached terms  */
static seq_t *terms = NULL;
static size_t term_count = 0;
static size_t term_cap   = 0;
static seq_t cur = 0;          /* next integer to test */

/* Ensure the cached array can hold at least 'need' elements. */
static void ensure_capacity(size_t need)
{
    if (need <= term_cap)
        return;
    size_t new_cap = (term_cap == 0) ? 16 : term_cap * 2;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp)          /* unrecoverable memory error */
        exit(1);
    terms    = tmp;
    term_cap = new_cap;
}

/* Return true if x has the same set of decimal digits as its
   digit‑square sum. */
static bool same_digit_set(seq_t x)
{
    unsigned char cnt[10];
    digit_counts(x, cnt);
    bool set_x[10] = {false};
    for (int d = 0; d < 10; ++d)
        set_x[d] = cnt[d] > 0;

    /* compute the sum of squares of digits */
    uint64_t sumsq = 0;
    for (int d = 0; d < 10; ++d)
        sumsq += (uint64_t)cnt[d] * (uint64_t)d * (uint64_t)d;

    /* digit set of the sum */
    bool set_s[10] = {false};
    if (sumsq == 0) {
        set_s[0] = true;
    } else {
        uint64_t t = sumsq;
        while (t > 0) {
            set_s[t % 10] = true;
            t /= 10;
        }
    }

    /* compare the two sets */
    for (int d = 0; d < 10; ++d)
        if (set_x[d] != set_s[d])
            return false;
    return true;
}

/* external function required by the framework */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                  /* undefined for n<1 – safe return */

    /* make sure the cache can accommodate the requested index */
    if ((size_t)n > term_cap)
        ensure_capacity((size_t)n);

    /* generate terms until we have at least n of them */
    while (term_count < (size_t)n) {
        if (same_digit_set(cur))
            terms[term_count++] = cur;
        cur += 1;
    }

    return terms[(size_t)n - 1];
}
