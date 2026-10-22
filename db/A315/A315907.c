/*
 * A315907  Coordination sequence Gal.3.40.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315907
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=6, a(2)=8, a(3)=14, a(4)=20;
 *            a(n)=a(n-1)+a(n-3)-a(n-4) for n>4
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Memoized implementation – works for any order of calls. */
#include <stdlib.h>

static seq_t *memo = NULL;   /* cache of previously computed values */
static size_t memo_len = 0;  /* length of memo (index+1 of last filled element) */

/* Helper to allocate or grow the cache to hold at least n+1 entries. */
static void
ensure_cache(idx_t n)
{
    size_t need = (size_t)n + 1;      /* number of entries required */
    if (need <= memo_len) return;    /* already large enough */

    /* grow the table: double size or to required size, whichever is larger */
    size_t new_cap = memo_len > 0 ? memo_len << 1 : 1;
    if (new_cap < need) new_cap = need;
    seq_t *new_mem = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
    if (!new_mem) { /* allocation failed – abort program */
        abort();
    }
    memo = new_mem;
    /* Initialize new entries with the recurrence */
    for (size_t i = memo_len; i < new_cap; ++i) {
        /* i >= 5 here because memo_len is at least 5 after init */
        memo[i] = memo[i-1] + memo[i-3] - memo[i-4];
    }
    memo_len = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Initial table for indices 0..4. */
    if (!memo) {
        memo_len = 5;
        memo = (seq_t *)malloc(memo_len * sizeof(seq_t));
        if (!memo) abort();
        memo[0] = (seq_t)1;
        memo[1] = (seq_t)6;
        memo[2] = (seq_t)8;
        memo[3] = (seq_t)14;
        memo[4] = (seq_t)20;
    }

    /* Expand cache if necessary and compute missing terms. */
    ensure_cache(n);

    return memo[(size_t)n];
}
