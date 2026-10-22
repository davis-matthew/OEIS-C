/*
 * A379672  Number of finite sets of positive integers with sum + product = n.
 * https://oeis.org/A379672
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of finite sets of positive integers with
 *            sum(S)+prod(S) = n (empty set allowed)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t, if needed */

/* Count subsets recursively */
static void dfs(idx_t start, seq_t sum, seq_t prod, idx_t limit, seq_t *cnt)
{
    seq_t val = sum + prod;
    if (val == (seq_t)limit) {
        (*cnt)++;
    }
    if (val >= (seq_t)limit) {
        return;
    }
    seq_t limitS = (seq_t)limit;
    for (idx_t next = start; next <= limit; ++next) {
        /* prune if product would exceed the limit */
        if (prod > limitS / (seq_t)next) {
            break;
        }
        seq_t newProd = prod * (seq_t)next;
        seq_t newSum = sum + (seq_t)next;
        seq_t newVal = newSum + newProd;
        if (newVal > limitS) {
            break;          /* further nexts will only increase newVal */
        }
        dfs(next + 1, newSum, newProd, limit, cnt);
    }
}

/* OEIS sequence A379672 */
seq_t a(idx_t n)
{
    seq_t cnt = 0;
    /* start recursion with the empty set (sum=0, prod=1) */
    dfs(1, (seq_t)0, (seq_t)1, n, &cnt);
    return cnt;
}
