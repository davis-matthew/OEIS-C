/*
 * A270295  Numbers that equal the sum of the squares of the digits of their
 *          own square in base 27.
 * https://oeis.org/A270295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   sum_sq_base27_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A270295: numbers that equal the sum of the squares of the digits
   of their own square in base 27. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/sum_sq_base27_idx.h"

/* Search bound.  For every seq_t the largest solution is < 20 000, so
   this fixed limit guarantees all terms are found. */
static const idx_t MAX_SEARCH = 20000;

/* Static storage for the discovered terms. */
static seq_t *seq      = NULL;
static size_t seq_count = 0;
static size_t seq_cap   = 0;
static bool seq_initialized = false;

/* Append a value to the dynamic array. */
static void append_seq(seq_t v)
{
    if (seq_count < seq_cap) {
        seq[seq_count++] = v;
    }
}

/* Initialise the static array with all terms up to MAX_SEARCH. */
static void init_sequence(void)
{
    if (seq_initialized)
        return;

    seq_initialized = true;
    seq_cap = (size_t)MAX_SEARCH;
    seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    if (!seq)
        return;               /* OOM – the caller will simply return 0 */

    for (idx_t m = 0; m <= MAX_SEARCH; ++m) {
        /* Skip numbers that cannot be represented exactly in seq_t. */
        if ((seq_t)m != m)
            continue;

        idx_t sq  = m * m;    /* safe in idx_t for m <= 20 000 */
        seq_t sum = sum_sq_base27_idx(sq);
        if (sum == (seq_t)m)
            append_seq((seq_t)m);
    }
}

/* External function required by the framework. */
seq_t a(idx_t n)
{
    init_sequence();

    /* a(1) is the first term (0).  Return 0 if n is out of range. */
    if (n == 0 || n > (idx_t)seq_count)
        return (seq_t)0;

    return seq[n - 1];
}
