/*
 * A308352  Number of k-ary quasitrivial semigroups that have no neutral
 *          element on an n-element set.
 * https://oeis.org/A308352
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A308352  –  Number of k‑ary quasitrivial semigroups with no neutral element
 *
 *  The sequence satisfies the recurrence
 *
 *      a(0) = 1          (temporary dummy value)
 *      a(1) = 0
 *      a(n) = n * a(n-1)
 *             + 2 * Σ_{k=0}^{n-2}  C(n , k) * a(k)          for n ≥ 2
 *
 *  The function below memoises all computed terms up to the largest
 *  requested index, so that calls in any order are answered in
 *  essentially O(n) time per new index.
 */

#include <stddef.h>
#include <stdlib.h>

/*  Function prototypes supplied by the build system.
 *  Do NOT modify these names. */
extern seq_t a(idx_t n);

/*  Static storage for memoisation  */
static idx_t    max_n   = 0;      /* largest n that has already been computed   */
static seq_t   *a_table = NULL;   /* array of computed a(k), 0 <= k <= max_n    */
static size_t   cap     = 0;      /* current capacity of a_table                 */

/*  Ensure that a_table has room for indices 0 … need  */
static void ensure_capacity(idx_t need)
{
    if ((idx_t)cap > need) return;
    size_t new_cap = cap ? cap : 4;
    while (new_cap <= (size_t)need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(a_table, new_cap * sizeof(seq_t));
    if (!tmp) return;            /* silently leave the old table in case of OOM */
    a_table = tmp;
    /* initialise new entries to zero (makes debugging easier) */
    for (size_t i = cap; i < new_cap; ++i) a_table[i] = (seq_t)0;
    cap = new_cap;
}

/*  Main function implementing the recurrence  */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* offset is 1 – we get no such requests, but be safe */

    if (n > max_n) {
        /*  Grow table to include the new index  */
        ensure_capacity(n);
        if (max_n == 0) {            /* first initialisation  */
            a_table[0] = (seq_t)1;   /* dummy value for the recurrence */
            a_table[1] = (seq_t)0;   /* a(1) = 0  */
            max_n     = 1;
        }
        /*  Compute all terms up to n  */
        for (idx_t i = max_n + 1; i <= n; ++i) {
            seq_t sum = (seq_t)0;
            /*  sum_{k=0}^{i-2} C(i,k)*a(k)  */
            seq_t binom = (seq_t)1;   /* C(i,0)  */
            for (idx_t k = 0; k <= i - 2; ++k) {
                sum += binom * a_table[k];
                /*  advance binom to C(i,k+1)  */
                /*  binom = binom * (i-k) / (k+1)  (exact division)  */
                binom = (binom * (seq_t)(i - k)) / (k + 1);
            }
            a_table[i] = (seq_t)i * a_table[i - 1] + (seq_t)2 * sum;
        }
        max_n = n;
    }
    return a_table[n];
}
