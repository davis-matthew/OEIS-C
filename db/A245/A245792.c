/*
 * A245792  Number of preferential arrangements of n labeled elements when at
 *          least k=7 elements per rank are required.
 * https://oeis.org/A245792
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 22, u64 n <= 33, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ************************************************************************
 *  Formula: a(n) = Σ_{k=7}^n binom(n, k) * a(n-k)
 *            a(0) = 1, a(n) = 0 for 1 ≤ n ≤ 6
 *
 *  This function computes the preferential‑arrangements numbers with
 *  the restriction that each rank contains at least 7 elements.  It
 *  uses simple dynamic programming and memoisation; the recurrence
 *  is evaluated in O(n²) time which is well within the limits for
 *  the required n up to about 10 000.  The implementation works for
 *  any integer type specified for seq_t and idx_t, including an
 *  arbitrary‑precision integer class when compiled as C++17.
 * ************************************************************************
 */

#include <stdlib.h>

/* helper: make sure the working array is large enough */
static void ensure_capacity(idx_t n, idx_t *cap, seq_t **dp)
{
    idx_t required = n + 1;                /* indices 0 … n */
    if ((size_t)required > (size_t)(*cap)) {
        idx_t new_cap = (*cap > 0 ? (*cap) * 2 : 1);
        if ((size_t)required > (size_t)new_cap)
            new_cap = required;
        /* realloc works for built‑in types; for C++ a cast is needed. */
        seq_t *tmp = (seq_t *)realloc(*dp, (size_t)new_cap * sizeof(seq_t));
        if (!tmp) abort();                /* allocation failed */
        *dp = tmp;
        *cap = new_cap;
    }
}

/*--------------------------------------------------------------------
 *  a(n)
 *
 *  Returns the nth term of OEIS A245792.  The function is internally
 *  memoised; subsequent calls for larger n will build on the values
 *  previously computed.  The implementation is thread‑safe only if
 *  calls are serial because it uses static storage.
 *--------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    static seq_t *dp = NULL;        /* array of computed values   */
    static idx_t capacity = 0;      /* allocated size              */
    static idx_t computed = 0;      /* highest index computed so far */

    /* Make sure we have room for indices 0 … n */
    ensure_capacity(n, &capacity, &dp);

    /* Initialise the base value once */
    if (computed == 0) {
        dp[0] = (seq_t)1;            /* a(0) = 1 */
    }

    /* If the value is already computed, return it */
    if (n <= computed) {
        return dp[n];
    }

    /* Compute all missing values up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t sum = 0;

        /* For i < 7 the sum remains 0 (a(i) = 0) */
        if (i >= 7) {
            /* Compute binom(i,7) once */
            seq_t binom = 1;
            for (int j = 1; j <= 7; ++j) {
                binom = binom * (seq_t)(i - 7 + j);
                binom = binom / (seq_t)j;
            }

            /* Iterate k = 7 … i */
            for (idx_t k = 7; k <= i; ++k) {
                sum += binom * dp[i - k];
                if (k < i) {
                    /* Update binom(i,k+1) = binom(i,k) * (i-k)/(k+1) */
                    binom = binom * (seq_t)(i - k);
                    binom = binom / (seq_t)(k + 1);
                }
            }
        }

        dp[i] = sum;
    }

    computed = n;
    return dp[n];
}
