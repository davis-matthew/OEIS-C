/*
 * A357860  Number of integer factorizations of n into distinct even factors.
 * https://oeis.org/A357860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts ways to write n as a product of distinct even
 *            factors (including the trivial factorization of n itself).
 *            a(1)=1, a(odd)=0 for n>1
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

static seq_t dfs(idx_t rem, idx_t min_even) {
    if ((rem & (idx_t)1) != (idx_t)0)  /* odd remainder cannot be factorized further */
        return (seq_t)0;
    seq_t total = (seq_t)0;
    for (idx_t d = min_even; d <= rem; d += (idx_t)2) {
        if (rem % d != (idx_t)0) continue;
        idx_t next = rem / d;
        if (next == (idx_t)1) {            /* finished: d is the last factor */
            total += (seq_t)1;
        } else {
            total += dfs(next, d + (idx_t)2);  /* next factor must be larger and even */
        }
    }
    return total;
}

seq_t a(idx_t n) {
    if (n == (idx_t)1) return (seq_t)1;   /* empty factorization of 1 */
    if ((n & (idx_t)1) != (idx_t)0)       /* odd n (except 1) has no even factors */
        return (seq_t)0;
    return dfs(n, (idx_t)2);
}
