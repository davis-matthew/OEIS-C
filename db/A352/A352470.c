/*
 * A352470  a(0) = 1; a(n) = Sum_{k=0..floor((n-1)/2)} binomial(n,2*k+1)^2 *
 *          a(n-2*k-1).
 * https://oeis.org/A352470
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor((n-1)/2)} binomial(n,2*k+1)^2 *
 *            a(n-2*k-1) Initial values: a(0) = 1 This implementation memoises
 *            past values so that calls can be made in any order
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* seq_t and idx_t are defined externally by the build system. */

seq_t a(idx_t n)
{
    /* Memoisation table: dp[i] holds a(i) for computed indices 0..computed-1 */
    static seq_t *dp        = NULL;
    static size_t capacity  = 0;   /* current allocation size (dp[0 .. capacity-1]) */
    static size_t computed  = 0;   /* number of values already computed; equals
                                      max index + 1 among computed entries */

    /* Allocate or grow the table to hold up to index n */
    if (dp == NULL) {
        capacity = (size_t)n + 1;
        dp = (seq_t*)malloc(capacity * sizeof(seq_t));
        if (dp == NULL) return (seq_t)0;  /* allocation failure – return 0 */
    } else if ((size_t)n + 1 > capacity) {
        capacity = (size_t)n + 1;
        dp = (seq_t*)realloc(dp, capacity * sizeof(seq_t));
        if (dp == NULL) return (seq_t)0;
    }

    /* Initialise a(0) if this is the first call */
    if (computed == 0) {
        dp[0] = (seq_t)1;
        computed = 1;
    }

    /* Compute all missing terms up to n */
    for (idx_t i = computed; i <= n; ++i) {
        seq_t sum  = (seq_t)0;
        seq_t bin  = (seq_t)1;           /* binomial coefficient C(i,0) */

        /* Iterate over j = 1 .. i to update bin = C(i,j) */
        for (idx_t j = 1; j <= i; ++j) {
            /* bin = bin * (i - j + 1) / j  (exact integer division) */
            bin = bin * (seq_t)(i - j + 1);
            bin = bin / (seq_t)j;

            if (j & 1) {                  /* j is odd → k = (j-1)/2 */
                seq_t prod = bin * bin;   /* (C(i,j))^2 */
                prod = prod * dp[i - j];  /* times a(i-j) */
                sum += prod;
            }
        }
        dp[i] = sum;
    }
    computed = (size_t)n + 1;

    return dp[n];
}
