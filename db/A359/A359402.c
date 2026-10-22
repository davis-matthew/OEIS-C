/*
 * A359402  Numbers whose binary expansion and reversed binary expansion have
 *          the same sum of positions of 1's, where positions in a sequence
 *          are read starting with 1 from the left.
 * https://oeis.org/A359402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A359402: numbers whose binary expansion and its reversal
   have the same sum of positions of the 1-bits (positions counted
   from the left starting at 1).  
   A term x satisfies 2*sum_positions = popcount(x) * (bitlen(x)+1). */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

static seq_t *terms = NULL;          /* cache of computed terms */
static size_t term_count = 0;        /* how many terms are cached */
static size_t capacity = 0;          /* current array capacity */
static seq_t next_candidate = 0;     /* next integer to test for membership */

/* Test whether x satisfies the property. */
static bool qualifies(seq_t x)
{
    seq_t sum = 0;          /* sum of positions of 1's */
    seq_t k = 0;            /* number of 1's */
    idx_t pos = 1;          /* current position from the left (1-based) */

    seq_t tx = x;
    while (tx != (seq_t)0) {
        if ((tx & (seq_t)1) != (seq_t)0) {
            k += (seq_t)1;
            sum += (seq_t)pos;
        }
        tx >>= 1;           /* shift right by one bit */
        pos++;
    }
    /* After loop, pos == length_of_binary + 1 */
    return (sum * (seq_t)2) == (k * (seq_t)pos);
}

/* Append a new term to the cache, expanding storage if needed. */
static void append_term(seq_t val)
{
    if (capacity == 0) {
        capacity = 16;
        terms = (seq_t *)malloc(capacity * sizeof(seq_t));
    } else if (term_count == capacity) {
        capacity *= 2;
        terms = (seq_t *)realloc(terms, capacity * sizeof(seq_t));
    }
    terms[term_count++] = val;
}

/* Compute the nth term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;   /* undefined for n = 0, but return 0 */
    }

    /* Ensure cache contains at least n terms. */
    while (term_count < n) {
        if (qualifies(next_candidate)) {
            append_term(next_candidate);
        }
        next_candidate += (seq_t)1;
    }

    return terms[n - 1];
}
