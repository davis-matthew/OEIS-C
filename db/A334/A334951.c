/*
 * A334951  a(n) is the smallest candidate for the n-th term of Recamán's
 *          sequence A005132.
 * https://oeis.org/A334951
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A005132(n-1) - n Recamán's candidate sequence A334951.
 *            The function computes the current value of Recamán's sequence
 *            incrementally and returns the corresponding candidate value
 * Range:     i8 n <= 67, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* static state for the Recamán recurrence */
static seq_t   recam_last = (seq_t)0;     /* last value A005132(k) */
static idx_t   recam_index = 0;           /* index of recam_last */
static bool *  visited = NULL;           /* visited non‑negative integers */
static size_t visited_cap = 0;           /* allocated capacity of visited */

/* ensure the visited array can hold indices up to 'needed' (inclusive) */
static void ensure_visited_cap(idx_t needed)
{
    if ((idx_t)visited_cap > needed)
        return;

    size_t new_cap = visited_cap ? visited_cap * 2 : 16;
    while ((idx_t)new_cap <= needed)
        new_cap <<= 1;   /* multiply by 2 while shrinking the loop */

    bool *tmp;
    if (visited == NULL) {
        tmp = (bool *)malloc(new_cap * sizeof(bool));
        if (!tmp) abort();
        for (size_t i = 0; i < new_cap; ++i)
            tmp[i] = false;
    } else {
        tmp = (bool *)realloc(visited, new_cap * sizeof(bool));
        if (!tmp) abort();
        for (size_t i = visited_cap; i < new_cap; ++i)
            tmp[i] = false;
    }
    visited = tmp;
    visited_cap = new_cap;
}

/* Advance Recamán's sequence up to index 'upto' (inclusive) */
static void ensure_recaman(idx_t upto)
{
    if (recam_index >= upto)
        return;

    /* initialise on first call */
    if (recam_index == 0 && recam_last == (seq_t)0 && visited == NULL) {
        ensure_visited_cap(0);
        visited[0] = true;
    }

    for (idx_t i = recam_index + 1; i <= upto; ++i) {
        seq_t cand = recam_last - (seq_t)i;
        bool use_cand = false;

        if (cand >= 0) {
            idx_t cand_idx = (idx_t)cand;
            ensure_visited_cap(cand_idx);
            if (!visited[cand_idx])
                use_cand = true;
        }

        seq_t next = use_cand ? cand : recam_last + (seq_t)i;

        recam_last = next;
        recam_index = i;

        if (next >= 0) {
            idx_t next_idx = (idx_t)next;
            ensure_visited_cap(next_idx);
            visited[next_idx] = true;
        }
    }
}

/* public function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    ensure_recaman(n - 1);

    return recam_last - (seq_t)n;
}
