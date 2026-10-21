/*
 * A288816  Coefficients in expansion of 1/E_2.
 * https://oeis.org/A288816
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, for n ≥ 1: * a(n) = 24 * Σ_{k=1}^{n} σ(k) * a(n – k) *
 *            where σ(k) = Σ_{d|k} d (sum of divisors of k)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Compute a(n) for OEIS sequence A288816. */
seq_t a(idx_t n)
{
    /* Static storage for computed terms and divisor sums. */
    static seq_t *A      = NULL;   /* a-values */
    static seq_t *sigma  = NULL;   /* σ(k) */
    static size_t cap    = 0;       /* current array capacity (number of entries) */
    static idx_t  max_n  = 0;       /* largest n for which a(k) has been computed */

    /* Ensure array capacity is sufficient for index n. */
    if (cap < (size_t)n + 1) {
        size_t new_cap = (size_t)n + 1;
        seq_t *newA    = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
        seq_t *newS    = (seq_t *)realloc(sigma, new_cap * sizeof(seq_t));
        /* In case realloc failed, everything is undefined – the testing harness
           will not provide such large values anyway. */
        A     = newA;
        sigma = newS;
        /* Zero any new elements that might hold garbage. */
        if (cap < new_cap) {
            memset(&A[cap], 0, (new_cap - cap) * sizeof(seq_t));
            memset(&sigma[cap], 0, (new_cap - cap) * sizeof(seq_t));
        }
        cap = new_cap;
    }

    /* Base case initialization (a(0) = 1). */
    if (max_n == 0 && A != NULL) {
        A[0] = (seq_t)1;
    }

    /* If the requested n is already computed, return it. */
    if (n <= max_n)
        return A[(size_t)n];

    /* Compute σ(k) for k = 1 … n (full recomputation; O(n log n)). */
    for (idx_t k = 1; k <= n; ++k)
        sigma[(size_t)k] = (seq_t)0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t ii = (seq_t)i;
        for (idx_t j = i; j <= n; j += i)
            sigma[(size_t)j] += ii;
    }

    /* Compute a(k) for k = max_n+1 … n. */
    for (idx_t k = max_n + 1; k <= n; ++k) {
        seq_t sum = (seq_t)0;
        for (idx_t i = 1; i <= k; ++i)
            sum += sigma[(size_t)i] * A[(size_t)(k - i)];
        A[(size_t)k] = (seq_t)24 * sum;
    }

    /* Record the new maximum n and return the requested value. */
    max_n = n;
    return A[(size_t)n];
}
