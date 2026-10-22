/*
 * A341069  Number of ways to write n as an ordered sum of 9 squarefree
 *          numbers.
 * https://oeis.org/A341069
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A341069:
 *     Number of ways to write n as an ordered sum of 9 squarefree numbers.
 * Formula:
 *     a(n) = [x^n] ( Sum_{k>=1} μ(k)^2 * x^k )^9
 *    (μ(n)^2 = 1 if n is squarefree, 0 otherwise)
 *
 * This implementation uses dynamic programming: for j=0…9
 *   dp[j][s]  = number of ways to write s as an ordered sum of j squarefree numbers.
 * The answer a(n) is dp[9][n] (for n >= 9; 0 elsewhere).
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#if defined(__cplusplus)
/* C++ build – allocate with new/delete to invoke constructors for big‑int types */
static inline seq_t *alloc_seq_arr(idx_t len)
{
    return new seq_t[(size_t)len];
}
static inline void free_seq_arr(seq_t *p)
{
    delete[] p;
}
#else
/* C build – simple malloc/free */
static inline seq_t *alloc_seq_arr(idx_t len)
{
    return (seq_t *)malloc((size_t)len * sizeof(seq_t));
}
static inline void free_seq_arr(seq_t *p)
{
    free(p);
}
#endif

static idx_t max_n = 0;                 /* largest n for which tables are built   */
static seq_t *dp[10] = {NULL};          /* dp[0] … dp[9] pointers               */
static unsigned char *sqfree = NULL;    /* flag array: 1 if squarefree, 0 otherwise */
static size_t sqfree_cap = 0;           /* current capacity of sqfree            */

/* Ensure that tables for all n <= n are computed (increasing n only). */
static void ensure_precomputed(idx_t n)
{
    if (n <= max_n) return;   /* already prepared */

    /* Free previous tables if any */
    for (int j = 0; j < 10; ++j) {
        if (dp[j]) {
            free_seq_arr(dp[j]);
            dp[j] = NULL;
        }
    }

    /* Allocate or enlarge squarefree flag array */
    if (sqfree_cap < (size_t)(n + 1)) {
        unsigned char *tmp = (unsigned char *)realloc(sqfree, (size_t)(n + 1));
        sqfree = tmp;
        sqfree_cap = (size_t)(n + 1);
    }

    /* Mark all numbers as squarefree first */
    for (idx_t i = 1; i <= n; ++i) sqfree[i] = 1;

    /* Erase numbers divisible by a square > 1 */
    for (idx_t p = 2; p * p <= n; ++p) {
        idx_t p2 = p * p;
        for (idx_t j = p2; j <= n; j += p2) sqfree[j] = 0;
    }

    /* Allocate tables for j = 0 … 9 */
    for (int j = 0; j < 10; ++j) {
        dp[j] = alloc_seq_arr(n + 1);
    }

    /* Base case: dp[0][0] = 1, others 0 */
    for (idx_t s = 0; s <= n; ++s) dp[0][s] = 0;
    dp[0][0] = 1;

    /* Build convolution tables for j = 1 … 9 */
    for (int j = 1; j <= 9; ++j) {
        seq_t *prev = dp[j - 1];
        seq_t *curr = dp[j];
        for (idx_t s = 0; s <= n; ++s) curr[s] = 0;

        for (idx_t k = 1; k <= n; ++k) {
            if (!sqfree[k]) continue;              /* only squarefree addends */
            idx_t limit = n - k;
            for (idx_t s = 0; s <= limit; ++s) {
                curr[k + s] += prev[s];
            }
        }
    }

    max_n = n;
}

seq_t a(idx_t n)
{
    if (n < 9) return 0;
    ensure_precomputed(n);
    return dp[9][n];
}
