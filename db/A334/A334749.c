/*
 * A334749  A fractal sequence produced by interleaving the sequence with its
 *          own ordinal transform.
 * https://oeis.org/A334749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1 For k >= 1: a(2k-1) = a(k) a(2k) = occurrence index of
 *            a(k) among a(1..k) (The occurrence index counts the element
 *            itself.) The sequence is memoised; on each call it is extended
 *            to the requested index, so calls may arrive in any order
 * Range:     u8 n <= 4487, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* seq_t and idx_t are provided by the build. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* offset is 1; value for n < 1 is arbitrary */

    /* static memoisation tables */
    static seq_t *seq = NULL;      /* a(1), a(2), ... */
    static size_t seq_len = 0;     /* number of elements stored so far */
    static size_t seq_cap = 0;     /* allocated capacity */

    /* helper to grow the table */
    if (!seq || n > seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 1;
        while (new_cap < (size_t)n)
            new_cap <<= 1;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }

    /* initialise first term */
    if (seq_len == 0) {
        seq[0] = (seq_t)1;
        seq_len = 1;
    }

    /* extend the sequence up to the requested index */
    while (seq_len < (size_t)n) {
        idx_t m = seq_len + 1;   /* 1‑based index of the new term */
        seq_t value;

        if (m & 1U) {            /* odd index: a(2k-1) = a(k) */
            idx_t k = (m + 1) >> 1;
            value = seq[k - 1];
        } else {                  /* even index: a(2k) = b(k) */
            idx_t k = m >> 1;
            seq_t target = seq[k - 1];

            /* count occurrences of target among a(1)..a(k) */
            idx_t occ = 1;        /* include a(k) itself */
            for (idx_t i = 0; i < k - 1; ++i)
                if (seq[i] == target)
                    ++occ;

            value = (seq_t)occ;
        }

        seq[seq_len] = value;
        ++seq_len;
    }

    return seq[n - 1];
}
