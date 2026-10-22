/*
 * A308037  a(n) = Sum_{d|n} Stirling2(n,d).
 * https://oeis.org/A308037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} Stirling2(n,d)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 17, u64 n <= 26, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
/* In C++ we allocate with new/delete. */
#define ALLOC_SEQ_ARRAY(n) new seq_t[(n)]
#define FREE_SEQ_ARRAY(p)  delete[] (p)
#else
/* In C we allocate with malloc/free. */
#define ALLOC_SEQ_ARRAY(n) ((seq_t*)malloc((n) * sizeof(seq_t)))
#define FREE_SEQ_ARRAY(p) (free((p)))
#endif

static seq_t* realloc_seq_array(seq_t *old, size_t old_cap, size_t new_cap) {
    seq_t *newp;
#ifdef __cplusplus
    newp = new seq_t[new_cap];
#else
    newp = (seq_t*)malloc(new_cap * sizeof(seq_t));
#endif
    size_t mincap = old_cap < new_cap ? old_cap : new_cap;
    for (size_t i = 0; i < mincap; ++i) newp[i] = old[i];
    if (old) FREE_SEQ_ARRAY(old);
    for (size_t i = mincap; i < new_cap; ++i) newp[i] = (seq_t)0;
    return newp;
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;   /* safety, not part of OEIS offset */

    /* Static state across calls */
    static seq_t *S = NULL;          /* current Stirling row */
    static size_t S_cap = 0;         /* allocated capacity for S */
    static idx_t cur_n = 0;          /* highest n for which S is ready */

    static seq_t *cache = NULL;      /* cached a(n) values */
    static size_t cache_cap = 0;     /* cached capacity */

    /* Ensure cache is large enough */
    if (cache_cap < (size_t)n + 1) {
        cache = realloc_seq_array(cache, cache_cap, (size_t)n + 1);
        for (size_t i = cache_cap; i <= (size_t)n; ++i) cache[i] = (seq_t)0;
        cache_cap = (size_t)n + 1;
    }

    /* Result already computed? */
    if (cur_n >= n) return cache[n];

    /* Ensure Stirling array is large enough */
    if (S_cap < (size_t)n + 1) {
        S = realloc_seq_array(S, S_cap, (size_t)n + 1);
        for (size_t i = S_cap; i <= (size_t)n; ++i) S[i] = (seq_t)0;
        S_cap = (size_t)n + 1;
    }
    S[0] = (seq_t)0;  /* base value used in recurrence */

    /* Build Stirling rows up to n */
    for (idx_t m = cur_n + 1; m <= n; ++m) {
        /* New row starts with S(m,m) = 1 */
        S[m] = (seq_t)1;

        /* Build from k = m-1 down to 1 */
        for (idx_t k = m - 1; k; --k) {
            seq_t t1 = S[k] * (seq_t)k;   /* old S(m-1,k) * k */
            seq_t t2 = S[k - 1];          /* old S(m-1,k-1) */
            S[k] = t1 + t2;               /* new S(m,k) */
        }

        /* Sum over divisors of m */
        seq_t sum = (seq_t)0;
        for (idx_t d = 1; d * d <= m; ++d) {
            if (m % d == 0) {
                sum += S[d];
                idx_t other = m / d;
                if (other != d) sum += S[other];
            }
        }
        cache[m] = sum;
    }

    cur_n = n;
    return cache[n];
}
