/*
 * A380310  Expansion of e.g.f. exp( 1 - 1/(1-5*x)^(1/5) ).
 * https://oeis.org/A380310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -∑_{k=1..n} A008548(k) * C(n‑1,k‑1) * a(n‑k) * with a(0)
 *            = 1, A008548(1) = 1 and * A008548(k) = A008548(k‑1) *
 *            (5*(k‑1)+1) (k ≥ 2)
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 8, i64 n <= 14, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/*  Ensure that the array *ptr has capacity at least 'needed'.
 *  'cap' stores the current capacity. */
static void ensure_capacity(size_t needed, size_t *cap, seq_t **ptr) {
    if (needed > *cap) {
        size_t new_cap = (*cap == 0) ? needed : (*cap);
        while (new_cap < needed) new_cap *= 2;
        *ptr = (seq_t *)realloc(*ptr, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* ------- a(n) cache ------------------------------------------- */
    static seq_t *a_cache = NULL;
    static size_t a_cap  = 0;      /* allocated capacity */
    static size_t a_len  = 0;      /* largest index stored */

    /* ------- A008548(k) cache ------------------------------------- */
    static seq_t *A_cache = NULL;
    static size_t A_cap  = 0;      /* allocated capacity */
    static size_t A_len  = 0;      /* largest index stored */

    /* --- initialise caches on first call -------------------------- */
    if (a_len == 0) {
        ensure_capacity(1, &a_cap, &a_cache);
        a_cache[0] = (seq_t)1;          /* a(0) = 1 */
        a_len = 0;
    }
    if (A_len == 0) {
        ensure_capacity(2, &A_cap, &A_cache);
        A_cache[1] = (seq_t)1;          /* A008548(1) = 1 */
        A_len = 1;
    }

    /* --- extend A008548 cache up to index n ----------------------- */
    if (A_len < n) {
        ensure_capacity((size_t)(n + 1), &A_cap, &A_cache);
        for (idx_t k = A_len + 1; k <= n; ++k) {
            /* A008548(k) = A008548(k-1) * (5*(k-1)+1) */
            seq_t factor = (seq_t)(5 * (k - 1) + 1);
            A_cache[k] = A_cache[k - 1] * factor;
        }
        A_len = n;
    }

    /* --- extend a(n) cache up to index n -------------------------- */
    if (a_len < n) {
        ensure_capacity((size_t)(n + 1), &a_cap, &a_cache);
        for (idx_t cur = a_len + 1; cur <= n; ++cur) {
            seq_t sum = 0;          /* sum of the convolution terms */
            seq_t bin = 1;          /* C(cur-1, 0) */

            for (idx_t k = 1; k <= cur; ++k) {
                seq_t term = A_cache[k] * bin;
                term *= a_cache[cur - k];
                sum += term;

                /* update binom for next k: C(cur-1, k) */
                if (k < cur) {
                    seq_t mult = (seq_t)(cur - k);
                    seq_t div  = (seq_t)k;
                    bin *= mult;
                    bin /= div;          /* exact division */
                }
            }
            a_cache[cur] = -sum;
        }
        a_len = n;
    }

    return a_cache[(size_t)n];
}
