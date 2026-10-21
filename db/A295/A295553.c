/*
 * A295553  Expansion of 1/(1 - Sum_{k>=1} (2*k-1)!!*x^k).
 * https://oeis.org/A295553
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n>0, a(n) = Sum_{k=1..n} (2*k-1)!! * a(n-k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, abort */

/* the function signature requested in the task */
seq_t a(idx_t n)
{
    /* Static caches shared between calls                      */
    static seq_t *A = NULL;          /* a(0)..a(A_size-1)    */
    static idx_t  A_size = 0;       /* number of computed a */
    static size_t A_cap  = 0;       /* allocated capacity    */

    static seq_t *W = NULL;          /* (2*k-1)!!, indices 1..W_size-1 */
    static idx_t  W_size = 0;        /* next unused index   */
    static size_t W_cap  = 0;        /* allocated capacity  */

    /* First call: initialise the caches                           */
    if (!A) {
        A_cap = 1;
        A = (seq_t*)calloc(A_cap, sizeof(seq_t));
        if (!A) abort();            /* cannot continue without memory */
        A[0] = 1;                   /* a(0) = 1                 */
        A_size = 1;

        W_cap = 2;                  /* need indices 0 and 1       */
        W = (seq_t*)calloc(W_cap, sizeof(seq_t));
        if (!W) abort();
        W[1] = 1;                   /* (2*1-1)!! = 1             */
        W_size = 2;                 /* next unused index = 2     */
    }

    /* If the value is already known, return it                   */
    if (n < A_size) return A[n];

    /* Ensure capacity for A                                           */
    if (A_cap <= (size_t)n) {
        size_t new_cap = A_cap ? A_cap << 1 : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(A, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        A = tmp;
        A_cap = new_cap;
    }

    /* Ensure capacity for W                                           */
    if (W_cap <= (size_t)n) {
        size_t new_cap = W_cap ? W_cap << 1 : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(W, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        W = tmp;
        W_cap = new_cap;
    }

    /* Compute (2*k-1)!! up to k = n, if needed                    */
    for (idx_t k = W_size; k <= n; ++k) {
        W[k] = W[k - 1] * (seq_t)(2 * k - 1);
    }
    W_size = n + 1;           /* now indices 1..n are ready */

    /* Compute a(i) for i from A_size up to n                       */
    for (idx_t i = A_size; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= i; ++k) {
            sum += W[k] * A[i - k];
        }
        A[i] = sum;
    }
    A_size = n + 1;

    return A[n];
}
