/*
 * A357661  Expansion of e.g.f. cosh( (exp(2*x) - 1)/sqrt(2) ).
 * https://oeis.org/A357661
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} 2^(n-k) * S(n,2k) * where S(n,k) is the
 *            Stirling number of the second kind
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Sequence types are supplied by the build system. */
 
/* ------------------------------------------------------------------ */
/*  Cache for Stirling numbers S[n][k]  (0 ≤ k ≤ n).  The table is
 *  grown as required by the function `ensure_up_to`.  All indices are
 *  of type idx_t, and all values are of type seq_t.                */
/* ------------------------------------------------------------------ */
static seq_t **S = NULL;          /* rows of Stirling numbers */
static idx_t  max_n = 0;          /* largest n for which rows exist */

/* Powers of two: pow2[i] = 2^i  (0 ≤ i ≤ max_pow2).  The table is
 * grown in parallel with `ensure_up_to`.                           */
static seq_t *pow2 = NULL;
static idx_t  max_pow2 = 0;       /* largest exponent stored */

/* ------------------------------------------------------------------ */
/* Ensure that Stirling numbers are available up to and including n
 * and that powers of two are available up to exponent n.  The
 * recurrence used is:
 *
 *   S[0][0] = 1
 *   S[n][0] = 0  (n>0)
 *   S[n][k] = k * S[n-1][k] + S[n-1][k-1]   for 1 ≤ k ≤ n
 *
 *  For k == n the term k * S[n-1][k] uses S[n-1][n] which is
 *  undefined (zero).  We guard against this by testing k ≤ n-1.      */
/* ------------------------------------------------------------------ */
static void ensure_up_to(idx_t n)
{
    /* First time: allocate the first row and the first power of two. */
    if (S == NULL) {
        S = (seq_t **)malloc(sizeof(seq_t *));
        S[0] = (seq_t *)malloc(sizeof(seq_t));
        S[0][0] = (seq_t)1;
        max_n = 0;

        pow2 = (seq_t *)malloc(sizeof(seq_t));
        pow2[0] = (seq_t)1;
        max_pow2 = 0;
    }

    if (n <= max_n)
        return;

    /* Grow the Stirling table row by row up to n. */
    S = (seq_t **)realloc(S, sizeof(seq_t *) * (n + 1));
    for (idx_t i = max_n + 1; i <= n; ++i) {
        S[i] = (seq_t *)malloc(sizeof(seq_t) * (i + 1));

        /* Base case: S[i][0] = 0 for i > 0. */
        S[i][0] = (seq_t)0;

        /* Recurrence for 1 ≤ k ≤ i. */
        for (idx_t k = 1; k <= i; ++k) {
            seq_t term1 = 0;
            if (k <= i - 1)            /* k * S[i-1][k] only defined when k ≤ i-1 */
                term1 = S[i - 1][k] * (seq_t)k;
            seq_t term2 = S[i - 1][k - 1];
            S[i][k] = term1 + term2;
        }
    }

    /* Ensure powers of two table covers exponent n. */
    if (n > max_pow2) {
        pow2 = (seq_t *)realloc(pow2, sizeof(seq_t) * (n + 1));
        for (idx_t i = max_pow2 + 1; i <= n; ++i)
            pow2[i] = pow2[i - 1] * ((seq_t)2);
        max_pow2 = n;
    }

    max_n = n;
}

/* ------------------------------------------------------------------ */
/* Compute a(n) according to the formula.  The function is safe to
 * call in any order since it memoises the necessary tables.           */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    ensure_up_to(n);

    seq_t result = (seq_t)0;
    for (idx_t k = 0; k <= n / 2; ++k) {
        idx_t m = 2 * k;                       /* column in Stirling table */
        seq_t term = pow2[n - k] * S[n][m];    /* 2^(n-k) * S(n,2k) */
        result += term;
    }
    return result;
}
