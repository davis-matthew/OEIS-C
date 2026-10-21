/*
 * A241792  Triangular numbers which have one or more occurrences of exactly
 *          nine different digits.
 * https://oeis.org/A241792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 493, u64 n <= 1500, u128 n <= 1500,
 *            big any n
 * Helpers:   append_candidate, digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A241792: returns the nth triangular number that contains exactly
 * nine distinct decimal digits.  Computed by iteratively testing
 * successive triangular numbers until the required count is found. */

#include <stddef.h>
#include <stdbool.h>

/* Utilities provided by the build environment */
#include "../utils/digit_counts.h"      /* void digit_counts(seq_t x, unsigned char cnt[10]); */
#include "../utils/append_candidate.h"  /* void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val); */

static seq_t *found_terms = NULL;
static size_t  found_len  = 0;
static size_t  found_cap  = 0;
static idx_t   last_i     = 0;

/* Return the nth term of A241792 (offset 1). */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* Undefined for n < 1 (offset 1). */

    /* Search until we have found at least n qualifying triangular numbers. */
    while (found_len < (size_t)n) {
        /* Next triangular number */
        last_i++;
        seq_t tri = (seq_t)last_i * ((seq_t)last_i + 1) / 2;

        /* Count distinct decimal digits of tri */
        unsigned char cnt[10];
        digit_counts(tri, cnt);

        int distinct = 0;
        for (int d = 0; d < 10; d++)
            if (cnt[d] != 0)
                distinct++;

        /* Record if exactly nine distinct digits are present */
        if (distinct == 9)
            append_candidate(&found_terms, &found_len, &found_cap, tri);
    }

    return found_terms[n - 1];
}
