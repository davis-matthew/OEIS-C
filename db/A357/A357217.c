/*
 * A357217  Array read by descending antidiagonals: T(n,k) is the number of
 *          cycles of the permutation given by the order of elimination in the
 *          Josephus problem for n numbers and a count of k; n, k >= 1.
 * https://oeis.org/A357217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 5050, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Validated: standard, pass, 107 terms through n = 4786 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Implementation of OEIS sequence A357217
 *
 *  a(n) is obtained by reading the 2‑D array T(n,k) of cycle counts of the
 *  Josephus permutation in descending antidiagonals.  For a given index m,
 *  let s be the smallest integer with (s-1)*s/2 >= m.  Within that
 *  antidiagonal (sum = s) the coordinates are
 *          n = 1 + (m - (s-2)*(s-1)/2 - 1),   k = s - n.
 *  The value a(m) equals the number of cycles of the Josephus permutation
 *  for parameters (n,k).  The cycle number is computed by a direct
 *  simulation of the elimination process.
 *
 *  The implementation uses only standard integer types and does not
 *  depend on any external utilities.
 */

#include <stdlib.h>
#include <stdbool.h>

/*  Count the cycles of the Josephus permutation for parameters (n,k).  */
static seq_t josephus_cycles(idx_t n, idx_t k)
{
    /*  labels[0..n-1] holds the remaining people.
        sigma[1..n] holds the permutation (elimination order).            */
    idx_t *labels = (idx_t *)malloc(n * sizeof(idx_t));
    idx_t *sigma  = (idx_t *)malloc((n + 1) * sizeof(idx_t));
    bool  *seen   = (bool  *)malloc((n + 1) * sizeof(bool ));
    if (!labels || !sigma || !seen) {   /*  ignore allocation failures  */
        return (seq_t)0;
    }
    for (idx_t i = 0; i < n; ++i) labels[i] = i + 1;
    for (idx_t i = 1; i <= n; ++i) seen[i] = false;

    idx_t sz = n;
    idx_t cur = 0;        /*  index into the current list of labels   */
    for (idx_t step = 1; step <= n; ++step) {
        cur = (cur + k - 1) % sz;           /*  elimination step               */
        idx_t lbl = labels[cur];            /*  person eliminated             */
        sigma[step] = lbl;

        /*  Remove the person from the list                          */
        for (idx_t j = cur; j < sz - 1; ++j)
            labels[j] = labels[j + 1];
        --sz;
        if (sz == 0) break;
    }

    /*  Count cycles of the permutation sigma[1..n]                */
    seq_t cycles = 0;
    for (idx_t i = 1; i <= n; ++i) {
        if (!seen[i]) {
            ++cycles;
            idx_t p = i;
            seen[p] = true;
            p = sigma[p];
            while (p != i) {
                seen[p] = true;
                p = sigma[p];
            }
        }
    }

    free(labels);
    free(sigma);
    free(seen);
    return cycles;
}

/*  Main sequence function
 *
 *  Input:   n – the index (1‑based) in the sequence.
 *  Output:  a(n) – the n‑th term.
 */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /*  undefined for 0 or negative      */

    /*  Find the antidiagonal (sum s) that contains index n              */
    idx_t s = 2;                         /*  smallest sum gives first entry   */
    while ((s - 1) * s / 2 < n) ++s;

    /*  Compute offset within the diagonal                               */
    idx_t prev_total = (s - 2) * (s - 1) / 2;
    idx_t offset   = n - prev_total - 1;        /*  zero‑based               */
    idx_t row_n    = 1 + offset;                 /*  n coordinate             */
    idx_t row_k    = s - row_n;                  /*  k coordinate             */

    return josephus_cycles(row_n, row_k);
}
