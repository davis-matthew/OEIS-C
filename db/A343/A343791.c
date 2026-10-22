/*
 * A343791  Number of ordered partitions of an n-set without blocks of size 8.
 * https://oeis.org/A343791
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n, k≠8} C(n,k) * a(n-k), a(0)=1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoisation */
    static seq_t *memo = NULL;
    static size_t memo_cap = 0;
    static idx_t memo_n = 0;

    /* grow storage if necessary */
    if (memo == NULL || memo_cap <= (size_t)n) {
        size_t new_cap = n + 1;
        if (new_cap < 2) new_cap = 2;
        if (new_cap > memo_cap * 2 && memo_cap > 0) new_cap = memo_cap * 2;
        seq_t *tmp = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failure */
        memo = tmp;
        memo_cap = new_cap;
    }

    /* initialise a(0) if first time */
    if (memo_n == 0 && memo[0] == 0) memo[0] = 1;

    /* compute values up to n */
    for (idx_t i = memo_n + 1; i <= n; ++i) {
        seq_t sum = 0;
        seq_t binom = 1;            /* C(i,0) */

        for (idx_t k = 1; k <= i; ++k) {
            /* update binom to C(i,k) from C(i,k-1) */
            binom = binom * ((seq_t)(i - k + 1));
            binom = binom / ((seq_t)k);

            if (k == 8) continue;   /* skip block of size 8 */
            sum += binom * memo[i - k];
        }
        memo[i] = sum;
    }
    if (memo_n < n) memo_n = n;
    return memo[n];
}
