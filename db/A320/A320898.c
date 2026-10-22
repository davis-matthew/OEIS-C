/*
 * A320898  Expansion of e.g.f. exp(theta_3(x) - 1), where theta_3() is the
 *          Jacobi theta function.
 * https://oeis.org/A320898
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 22,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A320898
   Recurrence (from the OEIS entry):
   a(0) = 1
   a(n) = Σ_{k∈S(n)} 2·k!·C(n–1, k–1)·a(n–k),  where S(n) = {m² | 1 ≤ m² ≤ n}
   (C(n–1, k–1) is the binomial coefficient).  */

/* ---------- utility headers for dynamic seq_t arrays ---------- */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

seq_t a(idx_t n)
{
    /* special case */
    if (n == (idx_t)0)
        return (seq_t)1;

    /* allocate arrays for factorials and the sequence values */
    seq_t *fact   = seq_alloc_new_array(n + 1);   /* fact[0..n]  */
    seq_t *a_arr  = seq_alloc_new_array(n + 1);   /* a[0..n]     */

    /* factorials */
    fact[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;

    /* pre‑compute squares up to n; store them in a small dynamic array */
    idx_t *squares = (idx_t*)malloc((n + 1) * sizeof(idx_t));
    idx_t sq_cnt = 0;
    for (idx_t m = 1; (m * m) <= n; ++m)
        squares[sq_cnt++] = m * m;

    /* dynamic programming for the sequence */
    a_arr[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;

        for (idx_t j = 0; j < sq_cnt && squares[j] <= i; ++j) {
            idx_t k = squares[j];                 /* k is a perfect square */

            /* binomial coefficient C(i-1, k-1) */
            seq_t bin = (seq_t)1;
            for (idx_t t = 1; t < k; ++t) {
                bin = bin * (seq_t)(i - k + t);
                bin = bin / (seq_t)t;
            }

            seq_t term = (seq_t)2 * fact[k];      /* 2·k!   */
            term = term * bin;                    /* *C(i-1,k-1) */
            term = term * a_arr[i - k];           /* *a(i-k) */
            sum = sum + term;
        }

        a_arr[i] = sum;
    }

    seq_t result = a_arr[n];

    /* free memory */
    seq_free_array(fact);
    seq_free_array(a_arr);
    free(squares);

    return result;
}
