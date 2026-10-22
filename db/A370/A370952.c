/*
 * A370952  a(1) = 1; for n > 1, a(n) is the least positive integer not
 *          already in the sequence such that a(n) == a(n-1) (mod 2*n-1).
 * https://oeis.org/A370952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 9293, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1;
   For n > 1, a(n) is the least positive integer not already in the sequence
   such that a(n) ≡ a(n-1) (mod 2*n-1).  */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Reallocate *arr so that it can hold at least *cap elements.  */
static void ensure_capacity(seq_t **arr, size_t *cap, size_t *len)
{
    if (*len >= *cap) {
        size_t new_cap = (*cap == 0) ? 8 : (*cap << 1);
        *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

/* Return the n-th term (n >= 1) of the sequence. */
seq_t a(idx_t n)
{
    /* Static state for memoisation. */
    static seq_t *terms    = NULL;   /* sequence values, 0‑based index */
    static size_t terms_len = 0;     /* number of terms stored       */
    static size_t terms_cap = 0;     /* allocated capacity for terms */

    static seq_t *used     = NULL;   /* table of values already used */
    static size_t used_len  = 0;     /* number of used values        */
    static size_t used_cap  = 0;     /* allocated capacity for used  */

    /* Initialise on first call. */
    if (terms_len == 0) {
        terms_cap = 16;
        used_cap  = 16;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        used  = (seq_t *)malloc(used_cap  * sizeof(seq_t));
        /* fail silently; the build system guarantees enough memory. */
        terms_len = 1;
        used_len  = 1;
        terms[0] = (seq_t)1;          /* a(1) */
        used[0]  = (seq_t)1;
    }

    /* Generate terms up to the requested index n (1‑based). */
    for (idx_t i = terms_len + 1; i <= n; ++i) {
        seq_t prev = terms[i - 2];                      /* a(i-1)          */
        seq_t m    = ((seq_t)i * 2) - 1;                /* 2*i-1           */

        /* Smallest positive integer ≡ prev (mod m). */
        seq_t r = prev % m;
        if (r == (seq_t)0)
            r = m;
        seq_t cand = r;

        /* Find the first candidate not already used. */
        for (;;) {
            bool used_flag = false;
            for (size_t j = 0; j < used_len; ++j) {
                if (cand == used[j]) { used_flag = true; break; }
            }
            if (!used_flag) break;  /* cand is new */
            cand += m;             /* try next residue */
        }

        /* Store the new term and record that it has been used. */
        ensure_capacity(&terms, &terms_cap, &terms_len);
        terms[terms_len++] = cand;

        ensure_capacity(&used, &used_cap, &used_len);
        used[used_len++] = cand;
    }

    return terms[n - 1];   /* a(n) */
}
