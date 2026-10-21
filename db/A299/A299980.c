/*
 * A299980  Lexicographic first sequence of positive integers such that
 *          a(n)*a(n+1) has a digit 0, and no term occurs twice.
 * https://oeis.org/A299980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   digit_counts, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A299980: Lexicographic first permutation of the positive integers
   such that a(n)*a(n+1) contains a digit 0 and no term occurs twice.  */

/* The function builds the sequence lazily.  Terms are memoised in a
   dynamic array.  For each new term the smallest unused positive
   integer that satisfies the product‑digit‑0 condition is appended.  */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_counts.h"
#include "../utils/seq_realloc.h"

static seq_t *seq = NULL;          /* array of computed terms          */
static size_t seq_count = 0;       /* number of terms currently stored */
static size_t seq_cap   = 0;       /* current capacity of the array    */

/* Ensure the array can hold at least sz terms.  */
static void ensure_cap(size_t sz)
{
    if (sz <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 64;
    while (new_cap < sz)
        new_cap *= 2;
    seq = seq_realloc(seq, seq_count, new_cap);
    seq_cap = new_cap;
}

/* Return true iff val has already been placed in seq[0..seq_count-1]. */
static bool already_used(seq_t val)
{
    for (size_t i = 0; i < seq_count; ++i)
        if (seq[i] == val)
            return true;
    return false;
}

/* Construct the nth term (1‑based).  */
seq_t a(idx_t n)
{
    if (n == 0)         /* the sequence is defined for n ≥ 1 */
        return (seq_t)0;

    /* initialise on the first call */
    if (seq_count == 0) {
        ensure_cap(1);
        seq[0] = (seq_t)1;
        seq_count = 1;
    }

    /* generate terms until we reach the desired index */
    while (seq_count < (size_t)n) {
        seq_t last = seq[seq_count - 1];

        for (idx_t cand = 1; ; ++cand) {
            seq_t cand_t = (seq_t)cand;

            if (already_used(cand_t))
                continue;                     /* duplicate, skip */

            /* product of the last term and the candidate */
            seq_t prod = last * cand_t;

            /* check that prod contains a digit 0 */
            unsigned char freq[10];
            digit_counts(prod, freq);
            if (freq[0] == 0)
                continue;                     /* no zero digit, skip */

            /* valid candidate found */
            ensure_cap(seq_count + 1);
            seq[seq_count] = cand_t;
            ++seq_count;
            break;
        }
    }

    return seq[n - 1];
}
