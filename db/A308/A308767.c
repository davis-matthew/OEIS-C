/*
 * A308767  Number of partitions of n into 4 squarefree parts.
 * https://oeis.org/A308767
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1..floor(n/4)} sum_{j=k..floor((n-k)/3)}
 *            sum_{i=j..floor((n-j-k)/2)} mu(k)^2 * mu(j)^2 * mu(i)^2 *
 *            mu(n-i-j-k)^2 The implementation counts unordered partitions of
 *            n into exactly 4 square‑free parts by dynamic programming over
 *            the square‑free numbers in increasing order
 * Range:     u8 n <= 53, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

#ifndef MAX_N
#define MAX_N 10000   /* Largest n that the static arrays can handle efficiently */
#endif

/* Static storage for the common case n <= MAX_N */
static seq_t dp0[MAX_N + 1];
static seq_t dp1[MAX_N + 1];
static seq_t dp2[MAX_N + 1];
static seq_t dp3[MAX_N + 1];
static seq_t dp4[MAX_N + 1];
static unsigned char is_sqfree[MAX_N + 1];
static idx_t sf[MAX_N + 1];

seq_t a(idx_t n)
{
    /* Small n that fits in the static buffer: clear and reuse it */
    if (n <= MAX_N) {
        size_t N = (size_t)n;

        /* Clear DP tables and the is_sqfree mask up to N */
        for (size_t i = 0; i <= N; ++i) {
            dp0[i] = dp1[i] = dp2[i] = dp3[i] = dp4[i] = 0;
            is_sqfree[i] = 1;
        }
        is_sqfree[0] = 0;    /* 0 is not square‑free */
        dp0[0] = 1;          /* empty multiset of zero parts sums to zero */

        /* Sieve to find square‑free numbers up to N */
        for (size_t i = 2; i * i <= N; ++i) {
            size_t sq = i * i;
            for (size_t j = sq; j <= N; j += sq)
                is_sqfree[j] = 0;
        }

        /* Build list of square‑free numbers up to N */
        size_t m = 0;
        for (size_t i = 1; i <= N; ++i)
            if (is_sqfree[i]) sf[m++] = (idx_t)i;

        /* DP: count partitions into exactly 4 parts */
        for (size_t idx = 0; idx < m; ++idx) {
            idx_t v = sf[idx];
            for (size_t s = (size_t)v; s <= N; ++s) {
                size_t offset = s - (size_t)v;
                dp1[s] += dp0[offset];
                dp2[s] += dp1[offset];
                dp3[s] += dp2[offset];
                dp4[s] += dp3[offset];
            }
        }

        return dp4[N];
    }

    /* Large n: fall back to dynamic allocation (used only rarely) */
    size_t N = (size_t)n;

    /* Allocate DP tables */
    seq_t *t0 = (seq_t*)calloc(N + 1, sizeof(seq_t));
    seq_t *t1 = (seq_t*)calloc(N + 1, sizeof(seq_t));
    seq_t *t2 = (seq_t*)calloc(N + 1, sizeof(seq_t));
    seq_t *t3 = (seq_t*)calloc(N + 1, sizeof(seq_t));
    seq_t *t4 = (seq_t*)calloc(N + 1, sizeof(seq_t));

    /* Allocate and initialise the square‑free mask */
    unsigned char *sqfree = (unsigned char*)calloc(N + 1, 1);
    if (!t0 || !t1 || !t2 || !t3 || !t4 || !sqfree) {
        free(t0); free(t1); free(t2); free(t3); free(t4); free(sqfree);
        return 0;
    }
    for (size_t i = 0; i <= N; ++i) sqfree[i] = 1;
    sqfree[0] = 0;
    t0[0] = 1;

    /* Sieve to find square‑free numbers up to N */
    for (size_t i = 2; i * i <= N; ++i) {
        size_t sq = i * i;
        for (size_t j = sq; j <= N; j += sq)
            sqfree[j] = 0;
    }

    /* Build list of square‑free numbers */
    idx_t *vec = (idx_t*)malloc((N + 1) * sizeof(idx_t));
    if (!vec) {
        free(t0); free(t1); free(t2); free(t3); free(t4); free(sqfree);
        return 0;
    }
    size_t m = 0;
    for (size_t i = 1; i <= N; ++i)
        if (sqfree[i]) vec[m++] = (idx_t)i;

    /* DP */
    for (size_t idx = 0; idx < m; ++idx) {
        idx_t v = vec[idx];
        for (size_t s = (size_t)v; s <= N; ++s) {
            size_t offset = s - (size_t)v;
            t1[s] += t0[offset];
            t2[s] += t1[offset];
            t3[s] += t2[offset];
            t4[s] += t3[offset];
        }
    }

    seq_t result = t4[N];
    free(t0); free(t1); free(t2); free(t3); free(t4);
    free(sqfree); free(vec);
    return result;
}
