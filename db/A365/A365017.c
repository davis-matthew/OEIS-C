/*
 * A365017  a(n) is the least nonnegative integer not already in the sequence
 *          whose binary expansion is not the concatenation of any two earlier
 *          terms.
 * https://oeis.org/A365017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   mask_from_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A365017 – a(1) = 0, a(n) is the least non‑negative integer not yet in the
 * sequence whose binary expansion is not the concatenation of two earlier terms.
 * This implementation builds the sequence incrementally, memoising results
 * for arbitrary call order. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/mask_from_len.h"

/* Auxiliary functions ---------------------------------------------- */

/* Return the number of bits needed to represent x in binary;
 * again, 0 needs one bit ("0"). */
static unsigned bitlen_seq_t(seq_t x)
{
    if (x == 0)
        return 1;
    unsigned len = 0;
    while (x != 0) {
        x >>= 1;
        ++len;
    }
    return len;
}

/* Binary search: does 'val' exist in the sorted array 'seq' of length 'len'? */
static bool seq_contains(const seq_t *seq, size_t len, seq_t val)
{
    size_t lo = 0, hi = len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        seq_t m = seq[mid];
        if (m < val)
            lo = mid + 1;
        else if (val < m)
            hi = mid;
        else
            return true;
    }
    return false;
}

/* Resize the dynamic array if needed. */
static void seq_grow(void);

/* Append 'val' to the sequence array. */
static void seq_push_back(seq_t val);

/* Check whether 'c' can be written as the concatenation of two distinct
 * earlier terms from the sequence. */
static bool is_concat(seq_t c);

/* ------------------------------------------------------------------- */

/* Persistent storage for the sequence. */
static seq_t *seq = NULL;   /* dynamic array of sequence terms */
static size_t  seq_len = 0; /* number of terms currently stored */
static size_t  seq_cap = 0; /* allocated capacity */
static seq_t   next_candidate = 0; /* next integer to test */

/* Grow the array if necessary. */
static void seq_grow(void)
{
    if (seq_len >= seq_cap) {
        size_t new_cap = seq_cap ? seq_cap << 1 : 16;
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp) abort();  /* malloc failure */
        seq  = tmp;
        seq_cap = new_cap;
    }
}

/* Append a new term to the sequence. */
static void seq_push_back(seq_t val)
{
    seq_grow();
    seq[seq_len++] = val;
}

/* Test whether 'c' is a concatenation of two distinct earlier terms. */
static bool is_concat(seq_t c)
{
    unsigned blen_c = bitlen_seq_t(c);
    /* Iterate over all earlier terms as potential right part. */
    for (size_t i = 0; i < seq_len; ++i) {
        seq_t y = seq[i];
        unsigned blen_y = bitlen_seq_t(y);
        if (blen_y > blen_c)
            break;                 /* remaining terms have longer binary form */
        seq_t mask = mask_from_len(blen_y);
        seq_t rhs = c & mask;
        if (rhs != y)
            continue;               /* right part does not match y */
        seq_t lhs = c >> blen_y;
        if (lhs == y)
            continue;               /* would use the same term twice */
        if (seq_contains(seq, seq_len, lhs))
            return true;            /* found a valid concatenation */
    }
    return false;
}

/* ------------------------------------------------------------------- */

/* External function required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* safety guard – sequence starts at 1 */
    size_t target = (size_t)n;           /* number of terms needed */

    /* Build terms until we have at least 'target' of them. */
    while (seq_len < target) {
        seq_t c = next_candidate;

        /* Skip if already in the sequence. */
        if (!seq_contains(seq, seq_len, c)) {
            /* Reject if c is a concatenation of two earlier terms. */
            if (!is_concat(c))
                seq_push_back(c);
        }

        ++next_candidate;                /* advance to next candidate */
    }

    return seq[n - 1];                    /* sequence is zero‑based internally */
}
