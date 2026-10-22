/*
 * A325465  Complement of A325464.
 * https://oeis.org/A325465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 106, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A325465: complement of A325464.
   A325465 consists of all numbers that can be written
   2x or 3x-2 for some x>1 in A325464.
   The following routine builds A325465 by iterating
   over the natural numbers and marking members
   using the recursive rule described above. */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *terms  = NULL;       /* stored sequence terms */
static size_t terms_cap = 0;
static idx_t  terms_len = 0;       /* number of terms currently stored */

static bool *marked = NULL;       /* true  → element is in A325465
                                      false → complement (A325464) */
static idx_t  marked_cap = 0;     /* array capacity */

static idx_t next_check = 3;      /* next integer to process */
static bool initialized = false;

/* Grow 'terms' array to hold at least 'new_cap' elements. */
static void ensure_terms_capacity(size_t new_cap)
{
    if (new_cap <= terms_cap) return;
    size_t cap = terms_cap ? terms_cap : 64;
    while (cap < new_cap) cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(terms, cap * sizeof(seq_t));
    if (!tmp) abort();               /* allocation failure */
    terms = tmp;
    terms_cap = cap;
}

/* Grow 'marked' array to hold at least elements [0..needed]. */
static void ensure_marked_capacity(idx_t needed)
{
    if (needed < marked_cap) return;
    size_t cap = marked_cap ? marked_cap : 64;
    while (cap <= (size_t)needed) cap <<= 1;
    bool *tmp = (bool *)realloc(marked, cap * sizeof(bool));
    if (!tmp) abort();
    /* initialize new slots to false */
    for (size_t i = marked_cap; i < cap; ++i) tmp[i] = false;
    marked = tmp;
    marked_cap = (idx_t)cap;
}

/* Fill data structures until we have at least 'n' terms. */
static void ensure_up_to_n(idx_t n)
{
    if (terms_len >= n) return;

    if (!initialized) {
        /* initialise marked array and put the mandatory base term 2 */
        ensure_marked_capacity(2);
        marked[0] = false;
        marked[1] = false;
        marked[2] = true;            /* A325465 contains 2 as a base case   */

        /* initialise terms array with the first term */
        ensure_terms_capacity(1);
        terms[0] = (seq_t)2;
        terms_len = 1;
        next_check = 3;              /* we have processed 1 and 2 already   */
        initialized = true;
    }

    while (terms_len < n) {
        /* Ensure marked array can index next_check */
        ensure_marked_capacity(next_check);

        /* If the current number is not in the sequence, it is a
           member of A325464 (complement).  For each such number >1
           generate its 2x and 3x-2 images and mark them as belonging
           to A325465.                         */
        if (!marked[next_check]) {          /* in the complement */
            if (next_check > 1) {
                idx_t v1 = next_check << 1;          /* 2 * next_check   */
                idx_t v2 = next_check * 3 - 2;        /* 3 * next_check - 2 */

                ensure_marked_capacity(v1);
                ensure_marked_capacity(v2);
                marked[v1] = true;
                marked[v2] = true;
            }
        }

        /* If the current number is marked, append it to the sequence. */
        if (marked[next_check]) {
            ensure_terms_capacity(terms_len + 1);
            terms[terms_len++] = (seq_t)next_check;
        }
        ++next_check;
    }
}

seq_t a(idx_t n)
{
    ensure_up_to_n(n);
    return terms[n - 1];
}
