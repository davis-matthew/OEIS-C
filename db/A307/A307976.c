/*
 * A307976  G.f. A(x) satisfies: A(x) = x * exp(Sum_{k>=1} (A(x^k) +
 *          sigma_2(k)*x^k)/k).
 * https://oeis.org/A307976
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=1, a(n+1) = (1/n) * Σ_{k=1..n} ( Σ_{d|k} d*(a(d)+d)
 *            ) * a(n-k+1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Auxiliary allocation helpers that work in both C and C++. */
#ifdef __cplusplus
# include <new>
static seq_t* allocate_seq_t_array(idx_t n)
{
    return new seq_t[n];
}
static void free_seq_t_array(seq_t* p)
{
    delete[] p;
}
#else
static seq_t* allocate_seq_t_array(idx_t n)
{
    return (seq_t*)malloc((size_t)n * sizeof(seq_t));
}
static void free_seq_t_array(seq_t* p)
{
    free(p);
}
#endif

/* Global dynamic arrays.  They are allocated on demand. */
static seq_t*  A    = NULL;   /* sequence values a(0..max_comp)      */
static seq_t*  S    = NULL;   /* auxiliary sums Σ_{d|k} d*(a(d)+d)   */
static idx_t   max_comp = 0; /* highest index computed so far     */
static idx_t   cap      = 0; /* current allocated capacity         */

/* Ensure the arrays can hold indices up to new_cap (inclusive). */
static void grow_to(idx_t new_cap)
{
    if (new_cap <= cap) return;

    seq_t* newA = allocate_seq_t_array(new_cap + 1);
    seq_t* newS = allocate_seq_t_array(new_cap + 1);

    /* Copy existing data. */
    for (idx_t i = 0; i <= max_comp; ++i) {
        newA[i] = A ? A[i] : (seq_t)0;
        newS[i] = S ? S[i] : (seq_t)0;
    }
    /* Initialise untouched slots. */
    for (idx_t i = max_comp + 1; i <= new_cap; ++i) {
        newA[i] = (seq_t)0;
        newS[i] = (seq_t)0;
    }

    /* Free old storage. */
    free_seq_t_array(A);
    free_seq_t_array(S);
    A = newA;
    S = newS;
    cap = new_cap;

    /* Update S for all divisors d that are already known. */
    for (idx_t d = 1; d <= max_comp; ++d) {
        seq_t contrib = (seq_t)d * ((seq_t)A[d] + (seq_t)d);
        idx_t start = max_comp + 1;
        idx_t j = ((start + d - 1) / d) * d;
        for (; j <= cap; j += d) {
            S[j] += contrib;
        }
    }
}

/* Initialise arrays for the first non‑zero terms. */
static void init_sequence(void)
{
    if (max_comp > 0) return;     /* already initialised */

    grow_to(1);                   /* allocate at least index 1   */
    A[0] = (seq_t)0;              /* a(0) = 0                    */
    A[1] = (seq_t)1;              /* a(1) = 1                    */
    max_comp = 1;

    /* Update S for divisor d = 1. */
    seq_t contrib = (seq_t)1 * ((seq_t)A[1] + (seq_t)1); /* 2 */
    for (idx_t j = 1; j <= cap; j += 1) {
        S[j] += contrib;                 /* only j = 1 when cap == 1 */
    }
}

/* Fill the sequence up to index n (inclusive). */
static void compute_up_to(idx_t n)
{
    while (max_comp < n) {
        idx_t curr = max_comp + 1;   /* index to compute: curr = n+1 */

        /* Compute the convolution sum Σ_{k=1}^{curr-1} S[k] * a(curr-k). */
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k < curr; ++k) {
            sum += S[k] * A[curr - k];
        }

        seq_t divisor = (seq_t)curr - (seq_t)1;
        A[curr] = sum / divisor;     /* exact integer division */

        /* Update S for new a[curr]. */
        seq_t contrib = (seq_t)curr * ((seq_t)A[curr] + (seq_t)curr);
        for (idx_t j = curr; j <= cap; j += curr) {
            S[j] += contrib;
        }

        max_comp = curr;
    }
}

/* Public function requested by the build system. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* a(0) = 0 */

    if (cap == 0) init_sequence();   /* first use initialises */
    if (n > cap)   grow_to(n);       /* ensure capacity */

    if (max_comp < n) compute_up_to(n);
    return A[n];
}
