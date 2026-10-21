/*
 * A268170  E.g.f. A(x) satisfies: A(x) = exp( Integral B(x) dx ) such that
 *          B(x) = exp(1+x - exp(x)) * exp( Integral A(x) dx ), where the
 *          constant of integration is zero.
 * https://oeis.org/A268170
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A268170 – computed from the functional equations 
   A'(x) = B(x) A(x) ,  B'(x) = B(x)[1 - exp(x) + A(x)]
   Recurrence for the EGF coefficients:
      a_{n+1} = Σ_{k=0}^{n} C(n,k) a_k b_{n-k}
      b_{n+1} = a_{n+1} - Σ_{k=0}^{n-1} C(n,k) b_k
   initial values: a_0 = b_0 = 1.
   The routine keeps a running cache so calls may come in any order.
   Complexity: O(n^2) arithmetic operations.  */

#include <stddef.h>
#include <stdlib.h>

/* static cache for the sequences */
static seq_t *A = NULL;          /* a(n) values   */
static seq_t *B = NULL;          /* b(n) values   */
static size_t capacity = 0;      /* allocated size   */
static size_t computed = 0;      /* number of entries computed (max index +1) */

/* Ensure the cache can hold values up to index n. */
static void ensure_capacity(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (need <= capacity)
        return;

    size_t new_cap = capacity ? capacity : 1;
    while (new_cap < need)
        new_cap <<= 1;           /* double until enough */

    A = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
    B = (seq_t *)realloc(B, new_cap * sizeof(seq_t));
    capacity = new_cap;
}

/* Initialise the cache with the base cases a_0 = b_0 = 1. */
static void init_cache(void)
{
    if (!A) {
        capacity = 1;
        A = (seq_t *)calloc(1, sizeof(seq_t));
        B = (seq_t *)calloc(1, sizeof(seq_t));
        A[0] = (seq_t)1;
        B[0] = (seq_t)1;
        computed = 1;   /* we have values for indices 0 .. computed-1 */
    }
}

/* Compute a(n) and the corresponding b(n) up to the requested index. */
seq_t a(idx_t n)
{
    init_cache();
    ensure_capacity(n);

    /* Compute missing entries in increasing order. */
    for (size_t idx = computed; idx <= (size_t)n; ++idx)
    {
        if (idx == 0)  /* already known */
            continue;

        /* Let m = idx-1; the recurrences use sums up to m. */
        size_t m = idx - 1;

        seq_t sumA = 0;           /* a_{idx} */
        seq_t sumDec = 0;         /* Σ_{k=0}^{m-1} C(m,k) b_k */
        seq_t bin = 1;            /* binomial coefficient C(m,0) */

        for (size_t k = 0; k <= m; ++k)
        {
            /* term for a_{idx} */
            seq_t termA = bin;
            termA *= A[k];
            termA *= B[m - k];
            sumA += termA;

            /* accumulate the subtraction part of b_{idx} */
            if (k < m)
                sumDec += bin * B[k];

            /* update binomial coefficient for next k (only if needed) */
            if (k < m)
            {
                bin = bin * (seq_t)(m - k);
                bin /= (seq_t)(k + 1);   /* exact division */
            }
        }

        A[idx] = sumA;
        B[idx] = sumA - sumDec;      /* b_{idx} */
    }

    computed = (size_t)n + 1;
    return A[(size_t)n];
}
