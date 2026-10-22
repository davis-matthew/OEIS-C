/*
 * A399553  a(n) is the least odd k such that neither k nor the odd part of k
 *          + 1 is yet in the sequence.
 * https://oeis.org/A399553
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   odd_part
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A399553: a(n) is the least odd k such that neither k nor the odd
 * part of k+1 has appeared before.
 * Computed by generating odd numbers and testing the odd_part
 * condition (a sieve). */

#include <stdlib.h>
#include "../utils/odd_part.h"

/* ------------------------------------------------------------------ */
/* Helper: dynamic array of seq_t values */
static struct {
    seq_t *data;
    size_t cnt;
    size_t cap;
} terms = {NULL, 0, 0};

static void ensure_terms_cap(size_t new_cap) {
    if (terms.cap >= new_cap) return;
    size_t cap = terms.cap ? terms.cap * 2 : 16;
    if (cap < new_cap) cap = new_cap;
    terms.data = (seq_t *)realloc(terms.data, cap * sizeof(seq_t));
    terms.cap = cap;
}

/* Binary search to test whether value is already in terms[0..cnt-1] */
static bool contains(const seq_t *arr, size_t cnt, seq_t value) {
    size_t lo = 0, hi = cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        seq_t midv = arr[mid];
        if (midv == value) return true;
        if (midv < value) lo = mid + 1;
        else hi = mid;
    }
    return false;
}

/* ------------------------------------------------------------------ */
/* Public function required by the framework */
seq_t a(idx_t n)
{
    /* lazily initialise storage */
    if (terms.data == NULL) {
        ensure_terms_cap(16);
        terms.cnt = 0;
        terms.cap = 16;
    }

    /* generate terms until we have at least n+1 entries */
    while (terms.cnt <= (size_t)n) {
        seq_t candidate;
        if (terms.cnt == 0) {
            candidate = 1;                     /* first odd number */
        } else {
            candidate = terms.data[terms.cnt - 1] + 2; /* next odd after last term */
        }

        /* find the next acceptable odd number */
        while (true) {
            seq_t op = odd_part(candidate + 1);
            if (!contains(terms.data, terms.cnt, op)) {
                break;                          /* candidate is admissible */
            }
            candidate = candidate + 2;          /* skip to next odd */
        }

        /* store the new term */
        ensure_terms_cap(terms.cnt + 1);
        terms.data[terms.cnt++] = candidate;
    }

    return terms.data[(size_t)n];
}
