/*
 * A376590  Second differences of consecutive squarefree numbers (A005117).
 *          First differences of A076259.
 * https://oeis.org/A376590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = s_{n+2} - 2*s_{n+1} + s_n where s_k is the k‑th
 *            squarefree number (s_1 = 1, s_2 = 2, …)
 * Range:     i8 n <= 76, i16 n <= 78, i32 n <= 78, i64 n <= 78, i128 n <= 78,
 *            big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_squarefree.h"

/* Ensure a dynamic array has space for at least 'needed' elements. */
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed > *cap) {
        size_t new_cap = (*cap == 0) ? 16 : *cap << 1;
        while (new_cap < needed) new_cap <<= 1;
        *ptr = realloc(*ptr, new_cap * elem_size);
        *cap = new_cap;
    }
}

/* ----------  global static state  ---------- */
static seq_t *s_arr = NULL;      /* 1‑based array of squarefree numbers   */
static size_t s_cap = 0;
static idx_t  s_count = 0;       /* number of squarefree numbers stored  */

static seq_t *a_arr = NULL;      /* 1‑based array of A376590 terms       */
static size_t a_cap = 0;
static idx_t  a_count = 0;       /* last index of computed a(n)         */

static idx_t last_checked = 1;   /* last integer inspected for squarefreeness */

/*  ----------  public function  ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for n<1; return 0 for safety */

    /* Lazily initialise the state on first call. */
    if (s_arr == NULL) {
        /* Start by storing the first squarefree number 1. */
        ensure_capacity(16, &s_cap, (void **)&s_arr, sizeof(seq_t));
        s_arr[1] = (seq_t)1;
        s_count = 1;

        /* Initialise the array that will hold a(n). */
        ensure_capacity(16, &a_cap, (void **)&a_arr, sizeof(seq_t));
    }

    /* Grow the sequences until we have computed a(n). */
    while (a_count < n) {
        /* Ensure we have enough squarefree numbers to evaluate the next a. */
        while (s_count <= a_count + 2) {
            /* Find the next squarefree integer after last_checked. */
            for (;;) {
                ++last_checked;
                if (is_squarefree((seq_t)last_checked)) {
                    ++s_count;
                    ensure_capacity((size_t)s_count + 1, &s_cap, (void **)&s_arr, sizeof(seq_t));
                    s_arr[s_count] = (seq_t)last_checked;
                    break;
                }
            }
        }

        /* Now we can compute a for index = a_count + 1. */
        idx_t idx = a_count + 1;
        seq_t val = s_arr[idx + 2] - 2 * s_arr[idx + 1] + s_arr[idx];
        ++a_count;
        ensure_capacity((size_t)a_count + 1, &a_cap, (void **)&a_arr, sizeof(seq_t));
        a_arr[idx] = val;
    }

    return a_arr[n];
}
