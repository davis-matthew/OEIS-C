/*
 * A296887  Numbers whose base-11 digits d(m), d(m-1), ..., d(0) have #(pits)
 *          < #(peaks); see Comments.
 * https://oeis.org/A296887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 6643, big any n
 * Helpers:   has_fewer_pits_than_peaks
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A296887 – Numbers whose base‑11 digits have fewer pits than peaks.
   Implementation: search for terms in increasing order, memoising
   all computed values in a dynamically resizable array.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_fewer_pits_than_peaks.h"

/* ------------------------------------------------------------------ */
/* Helper: count pits and peaks in the base‑11 representation of x   */

/* ------------------------------------------------------------------ */
/* Grow the static array of terms to at least new_cap elements.        */
/* ------------------------------------------------------------------ */
static void grow_terms(size_t new_cap)
{
    static seq_t *terms;
    static size_t cap = 0;

    /* First call – initialise pointers.  */
    if (cap == 0) {
        terms = (seq_t *)malloc(new_cap * sizeof(seq_t));
        cap   = new_cap;
    } else if (new_cap > cap) {
        terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        cap   = new_cap;
    }
}

/* ------------------------------------------------------------------ */
/* The main sequence function.  It is independent of call order.       */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* static storage for the pre‑computed terms              */
    static seq_t *terms = NULL;
    static size_t len   = 0;
    static size_t cap   = 0;

    /* static counter of the largest integer that has been tested */
    static uint64_t checked = 0;

    if (n == 0)   /* the OEIS offset is 1; return 0 for n==0 per spec */
        return (seq_t)0;

    /* initialise the array on first call */
    if (cap == 0) {
        cap   = 128;
        terms = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* generate terms until we have at least n of them */
    while (len < (size_t)n) {
        ++checked;
        if (has_fewer_pits_than_peaks(checked)) {
            if (len == cap)        /* enlarge storage */
                grow_terms(cap * 2);
            terms[len++] = (seq_t)checked;
        }
    }

    return terms[n - 1];
}
