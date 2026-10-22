/*
 * A368792  a(n) = (2*n-1)!! * Sum_{k=0..n} k/(2*k-1)!!.
 * https://oeis.org/A368792
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0; for n≥1, a(n) = (2*n-1)*a(n-1)+n
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *memo = NULL;      /* cache of computed values */
static idx_t memo_cap = 0;      /* allocated capacity   */
static idx_t memo_last = 0;     /* highest index already computed */

/* ensure memo has room for indices ≤ n (inclusive) */
static void ensure_capacity(idx_t n)
{
    if (n < memo_cap)
        return;
    size_t newcap = memo_cap ? memo_cap : 1;
    while (newcap <= (size_t)n)
        newcap <<= 1;
    seq_t *tmp = (seq_t *)realloc(memo, newcap * sizeof(seq_t));
    if (!tmp) abort();
    if (newcap > memo_cap)
        for (size_t i = memo_cap; i < newcap; ++i)
            tmp[i] = 0;
    memo = tmp;
    memo_cap = (idx_t)newcap;
}

/* a(n) with memoization */
seq_t a(idx_t n)
{
    if (!memo) {
        memo = (seq_t *)calloc(1, sizeof(seq_t));
        if (!memo) abort();
        memo[0] = 0;
        memo_cap = 1;
        memo_last = 0;
    }

    if (n <= memo_last)
        return memo[n];

    ensure_capacity(n);

    for (idx_t i = memo_last + 1; i <= n; ++i) {
        /* cast i to seq_t only when used in arithmetic */
        seq_t term = (seq_t)(2 * i - 1) * memo[i - 1] + (seq_t)i;
        memo[i] = term;
    }
    memo_last = n;
    return memo[n];
}
