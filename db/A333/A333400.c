/*
 * A333400  Lexicographically earliest infinite sequence of distinct integers
 *          whose partial sums are all distinct integers.
 * https://oeis.org/A333400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 161, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A333400: Lexicographically earliest infinite sequence of distinct integers with all partial sums distinct.
   The algorithm keeps a growing list of the sequence terms and of the partial sums.  
   For each new term it tries the integers in order of increasing magnitude 
   (0, -1, 1, -2, 2, -3, 3, …) and chooses the first one that is not already used
   and that does not create a duplicate partial sum. */

#include <stdlib.h>

/* Ensure arrays have capacity for 'need' elements.
   Arrays are of type seq_t, which may be built‑in or a user defined
   arbitrary‑precision integer class.  The cast to (seq_t *) is required
   for C++ compatibility. */
static void ensure_capacity(size_t need, seq_t **arr, size_t *cap)
{
    if (*cap < need) {
        size_t newcap = (*cap) ? (*cap) << 1 : 4;
        if (newcap < need) newcap = need;
        *arr = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
        *cap = newcap;
    }
}

/* Linear search for existence in an array of 'len' elements. */
static int contains(const seq_t *arr, size_t len, seq_t key)
{
    for (size_t i = 0; i < len; ++i)
        if (arr[i] == key) return 1;
    return 0;
}

/* External function required by the framework. */
seq_t a(idx_t n)
{
    /* Persistent state across calls */
    static seq_t *terms = NULL;       /* sequence terms in order of appearance   */
    static size_t terms_cap = 0, terms_len = 0;
    static seq_t *sums  = NULL;       /* record of partial sums                */
    static size_t sums_cap = 0, sums_len = 0;
    static seq_t cur_sum = 0;          /* sum of all computed terms             */

    /* Build the sequence up to the requested length. */
    while (terms_len < (size_t)n) {
        /* Try candidate integers in lexicographic order of magnitude. */
        for (idx_t cand_idx = 0; ; ++cand_idx) {
            seq_t cand;
            if (cand_idx == 0) {
                cand = 0;
            } else {
                seq_t mag = (seq_t)((cand_idx + 1) >> 1); /* positive magnitude */
                if (cand_idx & 1)
                    cand = -mag;
                else
                    cand = mag;
            }

            /* Check that the candidate is unused. */
            if (contains(terms, terms_len, cand))
                continue;

            /* Check that adding the candidate keeps all partial sums distinct. */
            seq_t new_sum = cur_sum + cand;
            if (contains(sums, sums_len, new_sum))
                continue;

            /* Candidate accepted.  Record it. */
            ensure_capacity(terms_len + 1, &terms, &terms_cap);
            ensure_capacity(sums_len + 1, &sums, &sums_cap);
            terms[terms_len++] = cand;
            sums[sums_len++]  = new_sum;
            cur_sum = new_sum;
            break; /* exit candidate loop */
        }
    }

    /* Return the (n‑1)‑th stored term (indices start at 1). */
    return terms[n - 1];
}
