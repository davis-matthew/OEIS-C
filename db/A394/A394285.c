/*
 * A394285  Number of ordered rooted trees with node weights summing to n;
 *          such that the root has weight 0, non-root nodes have positive
 *          integer weights, and only nodes of even weight can have branches.
 * https://oeis.org/A394285
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A394285 – Number of ordered rooted trees with node weights summing to n.
    The code uses the following recurrences derived from the stated generating
    function.
    Let s[n] be defined for n ≥ 1 by:
        s[0] = 0,
        s[n] = Σ_{i=1}^{n-1} s[i] * s[n-i]
               - Σ_{odd k < n} s[n-k]
               + 1                     (1)
    Then the desired sequence a[n] for n ≥ 0 satisfies:
        a[0] = 1,
        a[n] = Σ_{i=1}^{n} s[i] * a[n-i]   (2)
    These recurrences compute the sequence exactly and are efficient enough
    for n up to about 10 000. */

#include <stdlib.h>   /* calloc, free */
#include <stddef.h>   /* size_t */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    idx_t N = n;
    /* Allocate memory for s[0..N] and a[0..N] */
    seq_t *s = (seq_t *)calloc(N + 1, sizeof(seq_t));
    seq_t *t = (seq_t *)calloc(N + 1, sizeof(seq_t));
    if (!s || !t) {
        /* Allocation failed – return 0 as a safe fallback */
        free(s);
        free(t);
        return (seq_t)0;
    }

    s[0] = 0;          /* unused, but helpful for bounds */
    /* Compute s[1..N] using recurrence (1) */
    for (idx_t k = 1; k <= N; ++k) {
        seq_t sum_conv = 0;
        /* Σ_{i=1}^{k-1} s[i] * s[k-i] */
        for (idx_t i = 1; i < k; ++i)
            sum_conv += s[i] * s[k - i];

        seq_t t_k = 0;
        /* Σ_{odd k' < k} s[k - k'] */
        for (idx_t odd = 1; odd < k; odd += 2)
            t_k += s[k - odd];

        s[k] = sum_conv - t_k + (seq_t)1;
    }

    /* Compute a[0..N] using recurrence (2) */
    t[0] = 1;            /* t is temporarily used as a[] */
    for (idx_t k = 1; k <= N; ++k) {
        seq_t sum_a = 0;
        for (idx_t i = 1; i <= k; ++i)
            sum_a += s[i] * t[k - i];
        t[k] = sum_a;
    }

    seq_t result = t[N];
    free(t);
    free(s);
    return result;
}
