/*
 * A296766  Numbers whose base-60 digits d(m), d(m-1), ..., d(0) have #(rises)
 *          > #(falls); see Comments.
 * https://oeis.org/A296766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 182, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Interpretation: a(n) is the nth integer whose base‑60 digits d(m)…d(0) have
   more rises (d(i) < d(i+1)) than falls (d(i) > d(i+1)).
   The function performs a linear scan of integers, keeps a static cache
   of the terms found so far, and returns the required term.
   Since the definition is not a closed formula, the code simply tests
   each candidate number until enough terms have been collected. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static bool in_sequence(seq_t x)
{
    seq_t t = x;
    /* hold remainders to later reverse for most–significant order */
    uint8_t rev[128];
    size_t k = 0;

    if (t == 0) return false;          /* 0 is not a positive integer */

    while (t > 0) {
        rev[k++] = (uint8_t)(t % (seq_t)60);
        t /= (seq_t)60;
    }

    /* count rises and falls in most–significant order */
    size_t rises = 0, falls = 0;
    for (size_t i = 0; i + 1 < k; ++i) {
        uint8_t a = rev[k - 1 - i];
        uint8_t b = rev[k - 2 - i];
        if (a < b) ++rises;
        else if (a > b) ++falls;
    }
    return rises > falls;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;       /* offset <= 1, never used but defensive */

    /* static cache of found terms */
    static seq_t *terms = NULL;
    static size_t len  = 0;   /* number of elements currently stored */
    static size_t cap  = 0;   /* capacity of the cache array */
    static seq_t next  = 1;   /* next integer to test */

    if (cap == 0) {   /* first initialisation */
        cap = 64;
        terms = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* gather terms up to index n (1‑based) */
    while (len < (size_t)n) {
        if (in_sequence(next)) {
            if (len == cap) {
                cap *= 2;
                terms = (seq_t *)realloc(terms, cap * sizeof(seq_t));
            }
            terms[len++] = next;
        }
        ++next;
    }
    return terms[n - 1];
}
