/*
 * A243011  Sum of the three largest parts in the partitions of 4n into 4
 *          parts.
 * https://oeis.org/A243011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + 3*a(n-3) - 6*a(n-4) + 6*a(n-5) -
 *            3*a(n-6) + 3*a(n-7) - 3*a(n-8) + a(n-9) Initial values: a(1)=3,
 *            a(2)=34, a(3)=159, a(4)=489, a(5)=1161, a(6)=2365, a(7)=4336,
 *            a(8)=7323, a(9)=11640 The recurrence is rewritten to avoid
 *            negative intermediate values: a(n) = (3*a(n-1)+3*a...
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *memo   = NULL;   /* cached values, index 1.. */
static idx_t   mem_len = 0;     /* maximum index cached */
static idx_t   mem_cap = 0;     /* allocated size (>= mem_cap + 1) */

/* cached initial 9 terms */
static const seq_t base[10] = {
    0LL, /* unused for 0 index */
    3, 34, 159, 489, 1161, 2365, 4336, 7323, 11640
};

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined, safe guard */

    /* allocate / grow cache as needed */
    if (mem_len == 0) {
        idx_t init_cap = (n > 9 ? n + 1 : 10); /* support index up to max(n,9) */
        memo = (seq_t*)calloc((size_t)init_cap, sizeof(seq_t));
        if (!memo) abort();                      /* OOM guard */

        mem_cap = init_cap;
        for (idx_t i = 1; i <= 9; ++i) memo[i] = base[i];
        mem_len = 9;
    } else if (n >= mem_cap) {
        /* grow to at least n+1 */
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t*)realloc(memo, (size_t)new_cap * sizeof(seq_t));
        if (!tmp) abort();
        /* zero the newly added part (not strictly needed but safe) */
        for (idx_t i = mem_cap; i < new_cap; ++i) tmp[i] = 0;
        memo = tmp;
        mem_cap = new_cap;
    }

    if (n <= mem_len) return memo[n];

    /* compute missing terms up to n */
    for (idx_t k = mem_len + 1; k <= n; ++k) {
        seq_t sum1 = memo[k - 1] * (seq_t)3
                   + memo[k - 3] * (seq_t)3
                   + memo[k - 5] * (seq_t)6
                   + memo[k - 7] * (seq_t)3
                   + memo[k - 9];
        seq_t sum2 = memo[k - 2] * (seq_t)3
                   + memo[k - 4] * (seq_t)6
                   + memo[k - 6] * (seq_t)3
                   + memo[k - 8] * (seq_t)3;
        memo[k] = sum1 - sum2;  /* guaranteed non‑negative */
    }
    mem_len = n;
    return memo[n];
}
