/*
 * A352010  a(0) = 1; a(n) = Sum_{k=0..floor((n-1)/2)} (-3)^k * a(k) *
 *          a(n-2*k-1).
 * https://oeis.org/A352010
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1; a(n) = Σ_{k=0..⌊(n-1)/2⌋} (-3)^k * a(k) * a(n-2*k-1)
 *            for n≥1 (Recurrence from OEIS A352010)
 * Range:     i8 n <= 8, i16 n <= 13, i32 n <= 24, i64 n <= 45, i128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache to allow arbitrary order of calls */
    static seq_t *memo = NULL;   /* array of a values */
    static size_t memo_cap = 0;  /* current allocated size */
    static idx_t memo_len = 0;   /* number of values stored (>= 1) */

    /* Ensure capacity for at least n+1 entries */
    if (memo == NULL) {
        memo_cap = (size_t)n + 1;
        memo = (seq_t *)malloc(memo_cap * sizeof(seq_t));
        if (memo == NULL) return (seq_t)0;  /* allocation failure, return 0 */
        memo[0] = (seq_t)1;  /* a(0)=1 */
        memo_len = 1;
    } else if ((size_t)n + 1 > memo_cap) {
        /* Grow the cache if necessary */
        size_t new_cap = memo_cap * 2;
        while ((size_t)n + 1 > new_cap) new_cap *= 2;
        seq_t *new_mem = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
        if (new_mem == NULL) return (seq_t)0;  /* allocation failure */
        memo = new_mem;
        memo_cap = new_cap;
    }

    /* Compute any missing values up to n */
    if (memo_len <= n) {
        for (idx_t i = memo_len; i <= n; ++i) {
            idx_t maxk = (i - 1) / 2;   /* floor((i-1)/2) */
            seq_t sum = 0;
            seq_t pow_neg3 = 1;        /* (-3)^k */
            for (idx_t k = 0; k <= maxk; ++k) {
                seq_t term = pow_neg3 * memo[k] * memo[i - 2 * k - 1];
                sum += term;
                pow_neg3 *= (seq_t)(-3);
            }
            memo[i] = sum;
        }
        memo_len = n + 1;
    }

    return memo[n];
}
