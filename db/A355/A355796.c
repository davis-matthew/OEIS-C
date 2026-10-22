/*
 * A355796  Row 3 of A355793.
 * https://oeis.org/A355796
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Row 3 of A355793 (OEIS A355796)
 * Formula: a(0)=1.  For n≥1 let
 *   c(0)=1,
 *   c(k)=c(k‑1)·(3*(k+3)‑1)   (k≥1);
 * then
 *   a(n)=c(n) – 8·∑_{i=0}^{n‑1} c(i)·a(n‑1‑i).
 * This follows from A(x)/(1‑8xA(x)) = Σ_{k≥0}c(k)x^k.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *a_arr = NULL;   /* a[0..] */
    static seq_t *c_arr = NULL;   /* c[0..] */
    static idx_t size  = 0;       /* current allocated size */
    static idx_t last  = 0;       /* last computed index (≥0) */

    /* --- first call: initialise storage and base values -- */
    if (size == 0) {
        size = 16;                      /* initial capacity */
        a_arr = (seq_t *)malloc(size * sizeof(seq_t));
        c_arr = (seq_t *)malloc(size * sizeof(seq_t));
        if (!a_arr || !c_arr) return 0; /* allocation failure: safe return */

        a_arr[0] = 1;                   /* a(0) */
        c_arr[0] = 1;                   /* c(0) */
        last     = 0;
    }

    /* --- grow arrays if needed --- */
    if (n >= size) {
        size_t newsize = (size_t)(n + 1);
        if (newsize < size * 2) newsize = size * 2;   /* grow by factor 2 */
        a_arr = (seq_t *)realloc(a_arr, newsize * sizeof(seq_t));
        c_arr = (seq_t *)realloc(c_arr, newsize * sizeof(seq_t));
        size  = newsize;
    }

    /* --- compute missing terms up to n --- */
    for (idx_t m = last + 1; m <= n; ++m) {
        /* compute c(m) */
        if (m == 0) {
            c_arr[0] = 1;
        } else {
            seq_t factor = (seq_t)3 * ((seq_t)m + 3) - 1;
            c_arr[m] = c_arr[m - 1] * factor;
        }

        /* sum_{i=0}^{m-1} c(i) * a(m-1-i) */
        seq_t sum = 0;
        for (idx_t i = 0; i < m; ++i) {
            sum += c_arr[i] * a_arr[m - 1 - i];
        }

        /* a(m) = c(m) - 8*sum */
        a_arr[m] = c_arr[m] - ((seq_t)8 * sum);
    }

    last = n;
    return a_arr[n];
}
