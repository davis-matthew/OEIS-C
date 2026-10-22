/*
 * A375950  Expansion of e.g.f. 1 / (5 - 4 * exp(x))^(3/2).
 * https://oeis.org/A375950
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} A000407(k) * Stirling2(n,k) where A000407(k)
 *            = (2k+1)! / k! = 2^k * (2k+1)!! (product of odd numbers up to
 *            2k+1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Cache size tracking */
static size_t array_cap = 0;
static idx_t  computed_max = 0;
static seq_t *B = NULL;   /* B[k] = A000407(k) */
static seq_t *S = NULL;   /* Stirling numbers of the second kind (current row) */
static seq_t *A = NULL;   /* a[n] cache */

/* Ensure the internal arrays can hold at least 'need' elements.  */
static void ensure_capacity(size_t need)
{
    if (need <= array_cap)
        return;

    /* Reallocate arrays, preserve existing data. */
    void *tmp;
    tmp = realloc(B, need * sizeof(seq_t));
    if (!tmp) abort();  /* allocation failed */
    B = (seq_t *)tmp;

    tmp = realloc(S, need * sizeof(seq_t));
    if (!tmp) abort();
    S = (seq_t *)tmp;

    tmp = realloc(A, need * sizeof(seq_t));
    if (!tmp) abort();
    A = (seq_t *)tmp;

    array_cap = need;
}

/* Main function computing the sequence term for index n.  */
seq_t a(idx_t n)
{
    /* First‑time initialization.  */
    if (array_cap == 0) {
        ensure_capacity(1);
        B[0] = (seq_t)1;
        S[0] = (seq_t)1;
        A[0] = (seq_t)1;          /* a(0) = 1 */
        computed_max = 0;
    }

    /* Return cached value if already computed. */
    if (n <= computed_max)
        return A[n];

    /* Resize arrays to accommodate up to n. */
    ensure_capacity((size_t)n + 1);

    /* Compute successive rows until we reach n. */
    for (idx_t curr = computed_max + 1; curr <= n; ++curr) {
        /* Compute A000407(curr) = B[curr-1] * 2 * (2*curr + 1).  */
        seq_t mul = (seq_t)(2 * curr + 1);
        B[curr] = B[curr - 1] * (seq_t)2 * mul;

        /* Update Stirling numbers of the second kind for this row.  */
        for (idx_t k = curr; k >= 1; --k) {
            /* S(curr,k) = k * S(curr-1,k) + S(curr-1,k-1)  */
            S[k] = S[k] * (seq_t)k + S[k - 1];
        }
        S[0] = (seq_t)0;          /* S(curr,0) = 0 for curr > 0 */

        /* Accumulate the sum for a(curr).  */
        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k <= curr; ++k)
            sum += B[k] * S[k];
        A[curr] = sum;
    }

    computed_max = n;
    return A[n];
}
