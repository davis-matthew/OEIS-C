/*
 * A295227  a(n) = sum_{k=0,...,[n/2]} s(n-k,k)^2, s = A048994, Stirling
 *          numbers of the first kind.
 * https://oeis.org/A295227
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=0,...,[n/2]} s(n-k,k)^2, s = A048994, Stirling
 *            numbers of the first kind
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Computes OEIS sequence A295227. */
seq_t a(idx_t n) {
    /* static state: memoised values and Stirling rows */
    static seq_t *a_vals = NULL;            /* a(0) .. a(computed_n) */
    static size_t a_cap  = 0;                /* capacity of a_vals */
    static idx_t  computed_n = 0;            /* largest n already computed */

    /* Stirling rows for previous two values */
    static seq_t *row0 = NULL;              /* s(n-2-k,k)   */
    static seq_t *row1 = NULL;              /* s(n-1-k,k)   */
    static seq_t *row2 = NULL;              /* temporary new row */
    static size_t row0_cap = 0;
    static size_t row1_cap = 0;
    static size_t row2_cap = 0;

    /* ensure storage for a_vals up to n */
    if (n >= a_cap) {
        size_t new_cap = a_cap ? a_cap * 2 : 2;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(a_vals, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failure */
        a_vals = tmp;
        a_cap = new_cap;
    }

    /* first call: initialise rows and base values */
    if (computed_n == 0) {
        row0 = (seq_t *)calloc(1, sizeof(seq_t));
        row1 = (seq_t *)calloc(1, sizeof(seq_t));
        row2 = (seq_t *)calloc(1, sizeof(seq_t));
        if (!row0 || !row1 || !row2) return (seq_t)0;  /* allocation error */
        row0_cap = row1_cap = row2_cap = 1;
        row0[0] = (seq_t)1;     /* s(0,0) */
        row1[0] = (seq_t)0;     /* s(1,0) */

        a_vals[0] = (seq_t)1;   /* a(0) */
        a_vals[1] = (seq_t)0;   /* a(1) */
        computed_n = 1;
    }

    /* compute missing values up to n */
    if (n > computed_n) {
        for (idx_t cur = computed_n + 1; cur <= n; ++cur) {
            idx_t Kmax = cur / 2;

            /* enlarge row2 if necessary */
            if (row2_cap < (size_t)Kmax + 1) {
                seq_t *tmp = (seq_t *)realloc(row2, ((size_t)Kmax + 1) * sizeof(seq_t));
                if (!tmp) return (seq_t)0; /* allocation error */
                row2 = tmp;
                row2_cap = (size_t)Kmax + 1;
            }

            idx_t bound1 = (cur - 1) / 2;
            idx_t bound2 = (cur - 2) / 2;
            for (idx_t k = 0; k <= Kmax; ++k) {
                seq_t term1 = (seq_t)0;
                seq_t term2 = (seq_t)0;
                if (k > 0 && k - 1 <= bound2) term1 = row0[k - 1];
                if (k <= bound1) term2 = (seq_t)(cur - k - 1) * row1[k];
                row2[k] = term1 + term2;
            }

            /* sum of squares */
            seq_t sum = (seq_t)0;
            for (idx_t k = 0; k <= Kmax; ++k)
                sum += row2[k] * row2[k];

            a_vals[cur] = sum;

            /* rotate rows: row0 <- row1, row1 <- row2, row2 <- old row0 */
            seq_t *tmp_ptr = row0;     size_t tmp_cap = row0_cap;
            row0 = row1;               row0_cap = row1_cap;
            row1 = row2;               row1_cap = row2_cap;
            row2 = tmp_ptr;            row2_cap = tmp_cap;
        }
        computed_n = n;
    }

    return a_vals[n];
}
