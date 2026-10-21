/*
 * A292935  Expansion of e.g.f.: exp(exp(-x) - 1).
 * https://oeis.org/A292935
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^n * A000110(n) (Bell numbers)
 * Range:     i8 n <= 5, i16 n <= 9, i32 n <= 15, i64 n <= 25, i128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/*  a(0) = 1, a(1) = -1, a(2) = 2, a(3) = -5, …  */

seq_t a(idx_t n)
{
    /* static cache for Bell numbers (B[0] = 1) */
    static seq_t *bell   = NULL;   /* array of calculated Bell numbers   */
    static idx_t bell_len = 0;     /* number of Bell numbers stored      */
    static size_t bell_cap = 0;    /* current allocation size            */

    /* Allocate / grow array as needed */
    idx_t needed = n + 1;          /* we need indices 0 … n */
    if (needed > bell_cap) {
        bell = (seq_t *)realloc(bell, needed * sizeof(seq_t));
        bell_cap = (size_t)needed;
    }

    /* First‑time initialisation */
    if (bell_len == 0) {
        bell[0] = (seq_t)1;   /* B(0) = 1 */
        bell_len = 1;
    }

    /* Compute missing Bell numbers up to n, iteratively using the
       recurrence                  B(i+1) = sum_{k=0}^{i} C(i,k) * B(k)   */
    for (idx_t i = bell_len; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        seq_t binom = (seq_t)1;       /* C(i-1,0) = 1 for k = 0 */

        for (idx_t k = 0; k < i; ++k) {
            if (k > 0) {
                /* binom = binom * (i - k) / k    (C(i-1,k) from C(i-1,k-1)) */
                binom = (binom * (seq_t)(i - k)) / (seq_t)k;
            }
            sum += binom * bell[k];
        }
        bell[i] = sum;
    }

    bell_len = n + 1;  /* now we have Bell numbers up to index n */

    /* apply the alternating sign : a(n) = (-1)^n * B(n) */
    return (n & 1) ? -(bell[n]) : bell[n];
}
