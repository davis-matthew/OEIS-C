/*
 * A318354  Triangle read by rows: T(n,k) is the number of permutations p of
 *          {1..n} such that p(1)=k and p(i+1) < p(i) iff a strict majority of
 *          {1..n} \ {p(1)..p(i)} are < p(i).
 * https://oeis.org/A318354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r , k) where r = smallest integer with r*(r+1)/2 ≥ n. k
 *            = n - r*(r-1)/2. The array T is defined by T(1,1)=1 and for n>1:
 *            let m = n-1 and h = m/2. if (k <= h+1) T(n,k) = Σ_{i=k}^{m}
 *            T(m,i); else T(n,k) = Σ_{i=1}^{k-1} T(m,i); (The definition
 *            yields the triangle in the OEIS entry A318354.)
 * Range:     u8 n <= 21, u16 n <= 55, u32 n <= 120, u64 n <= 276,
 *            u128 n <= 703, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static seq_t *tri_rows[200];          /* pointers to rows of the triangle   */
static size_t tri_len = 0;           /* number of rows currently computed */

/* Compute up to at least row "need".  The triangle is stored in tri_rows. */
static void ensure_rows_to(size_t need) {
    while (tri_len < need) {
        /* Compute next row based on previous row */
        size_t prev_len = tri_len;                /* length of previous row   */
        size_t next_len = prev_len + 1;           /* length of new row        */

        seq_t *next_row = (seq_t *)malloc(sizeof(seq_t) * next_len);
        if (!next_row) { abort(); }              /* allocation failed       */

        if (tri_len == 0) {                      /* base row (n = 1)        */
            next_row[0] = (seq_t)1;
        } else {
            /* Prefix sums of previous row for fast range sums */
            seq_t *prefix = (seq_t *)malloc(sizeof(seq_t) * (prev_len + 1));
            if (!prefix) { abort(); }
            prefix[0] = (seq_t)0;
            for (size_t i = 1; i <= prev_len; ++i) {
                prefix[i] = prefix[i-1] + tri_rows[tri_len-1][i-1];
            }

            size_t half = prev_len / 2;          /* m/2 for the recurrence   */
            seq_t total = prefix[prev_len];      /* Σ_{i=1}^{m} T(m,i)      */

            for (size_t k = 1; k <= next_len; ++k) {
                if (k <= half + 1) {
                    /* sum over i = k .. m */
                    next_row[k-1] = total - prefix[k-1];
                } else {
                    /* sum over i = 1 .. k-1 */
                    next_row[k-1] = prefix[k-1];
                }
            }
            free(prefix);
        }

        /* Store the new row */
        tri_rows[tri_len] = next_row;
        tri_len++;
    }
}

/* Returns the n-th term of the sequence (1‑based). */
seq_t a(idx_t n) {
    if (n < (idx_t)1) return (seq_t)0; /* undefined for n <= 0, return 0 */

    /* Find the row that contains index n.  The cumulative count before row r is
       (r-1)*r/2. */
    size_t r = 1;
    while ((idx_t)(r * (r + 1)) / 2 < n) {
        r++;
    }

    ensure_rows_to(r);

    idx_t prev_total = (idx_t)(r * (r - 1)) / 2;
    size_t k = (size_t)(n - prev_total);  /* 1‑based column index within row r */

    return tri_rows[r-1][k-1];
}
