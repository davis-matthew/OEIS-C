/*
 * A345884  G.f. A(x) satisfies: A(x) = x * exp(2 * Sum_{k>=1} (-1)^k * A(x^k)
 *          / k).
 * https://oeis.org/A345884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1)=1; for n≥1, a(n+1) = (2/n) * ∑_{k=1..n} (∑_{d|k} (-1)^(k/d)
 *            * d * a(d)) * a(n-k+1)
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 14, i64 n <= 25, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static storage for memoization */
static seq_t *A = NULL;        /* a array, indices 0..CAP-1 correspond to a(1)..a(CAP) */
static seq_t *INNER = NULL;   /* inner array, indices 1..CAP correspond to inner(k) */
static idx_t USED = 0;        /* number of terms currently stored in A */
static idx_t CAP  = 0;        /* current capacity of A and INNER */

/* forward declarations */
static seq_t compute_inner(idx_t k);
static void ensure_capacity(idx_t n);

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)                 /* off‑offset, if called return 0 */
        return (seq_t)0;

    ensure_capacity(n);

    if (USED == 0)              /* initialise a(1) and inner(1) */
    {
        A[0] = (seq_t)1;
        INNER[1] = compute_inner(1);
        USED = 1;
    }

    /* compute missing terms up to n */
    while (USED < n)
    {
        idx_t i = USED + 1;          /* i is the next index to compute (a(i)) */

        seq_t sum1 = (seq_t)0;
        for (idx_t k = 1; k < i; ++k)
            sum1 += INNER[k] * A[i - k - 1];

        seq_t denom = (seq_t)(i - 1);
        seq_t ai = ((seq_t)2 * sum1) / denom;

        A[USED] = ai;                /* A[USED] corresponds to a(i) */
        INNER[i] = compute_inner(i);
        ++USED;
    }

    return A[n - 1];
}

/* Compute INNER[k] = Σ_{d|k} (-1)^(k/d) * d * a(d) */
static seq_t compute_inner(idx_t k)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= k; ++d)
    {
        if (k % d == 0)
        {
            idx_t d1 = d;
            idx_t d2 = k / d;
            idx_t q1 = k / d1;      /* exponent for sign of first divisor */
            seq_t term1 = (seq_t)d1 * A[d1 - 1];
            if (q1 & 1) term1 = -term1;
            sum += term1;

            if (d2 != d1)
            {
                idx_t q2 = k / d2;  /* equals d1 */
                seq_t term2 = (seq_t)d2 * A[d2 - 1];
                if (q2 & 1) term2 = -term2;
                sum += term2;
            }
        }
    }
    return sum;
}

/* Ensure that the memoization arrays have capacity at least n. */
static void ensure_capacity(idx_t n)
{
    if (CAP >= n)
        return;

    idx_t new_cap = CAP ? CAP * 2 : 8;
    while (new_cap < n)
        new_cap *= 2;

    seq_t *tmp_A     = (seq_t *)realloc(A,     new_cap * sizeof(seq_t));
    seq_t *tmp_INNER = (seq_t *)realloc(INNER, (new_cap + 1) * sizeof(seq_t));

    A     = tmp_A;
    INNER = tmp_INNER;
    CAP   = new_cap;
}
