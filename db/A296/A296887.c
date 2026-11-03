/*
 * A296887  Numbers whose base-11 digits d(m), d(m-1), ..., d(0) have #(pits)
 *          < #(peaks); see Comments.
 * https://oeis.org/A296887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 68 terms through n = 9869 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A296887 – Numbers whose base‑11 digits have fewer pits than peaks.
   Implementation: search for terms in increasing order, memoising
   all computed values in a dynamically resizable array.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* ------------------------------------------------------------------ */
static bool has_fewer_pits_than_peaks(uint64_t x)
{
    uint8_t digits[64];   /* enough for all 64‑bit numbers in base 11       */
    size_t len = 0;

    /* Extract digits (least significant first).  */
    do {
        digits[len++] = (uint8_t)(x % 11);
        x /= 11;
    } while (x > 0);

    if (len < 3)                 /* no interior positions → no pits/peaks    */
        return false;

    size_t pits  = 0;
    size_t peaks = 0;
    for (size_t i = 1; i + 1 < len; ++i) {
        uint8_t a = digits[i-1];
        uint8_t b = digits[i];
        uint8_t c = digits[i+1];
        if (a > b && b < c)  ++pits;
        else if (a < b && b > c)  ++peaks;
    }
    return pits < peaks;
}

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
        terms = mem_new(seq_t, new_cap);
        cap   = new_cap;
    } else if (new_cap > cap) {
        terms = mem_resize(seq_t, terms, new_cap);
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
        terms = mem_new(seq_t, cap);
    }

    /* generate terms until we have at least n of them */
    while (len < (size_t)n) {
        ++checked;
        if (has_fewer_pits_than_peaks(checked)) {
            if (len == cap)        /* enlarge storage */
                grow_terms(cap * 2);
            if (len >= cap) {                 /* grow before storing */
                cap = cap ? 2 * cap : 16;
                seq_t *grown_ = mem_resize(seq_t, terms, cap);
                if (!grown_) abort();
                terms = grown_;
            }
            terms[len++] = (seq_t)checked;
        }
    }

    return terms[n - 1];
}
