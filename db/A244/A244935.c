/*
 * A244935  Number of length n 0..3 arrays with each partial sum starting from
 *          the beginning no more than sqrt(3) standard deviations from its
 *          mean.
 * https://oeis.org/A244935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length n arrays with elements in {0,1,2,3} such
 *            that for every k (1 ≤ k ≤ n) the partial sum S_k satisfies (2
 *            S_k − 3 k)² ≤ 15 k. Offset: 1 (a(1) is the first term)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* floor(sqrt(n)) for unsigned 64‑bit n */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t res = 0;
    idx_t bit = ((idx_t)1) << 62;          /* highest even power of 2 ≤ 2^64/2 */
    while (bit > 0) {
        idx_t next = res | bit;
        if (next <= n / next) {
            res = next;
        }
        bit >>= 1;
    }
    return res;
}

/* a(n): number of 0..3 arrays of length n satisfying the partial‑sum bound */
seq_t a(idx_t n)
{
    static seq_t *a_vals = NULL;          /* a_vals[i-1] = a(i) */
    static size_t aSize = 0;              /* number of cached values */
    static idx_t maxComputed = 0;         /* largest n computed so far */

    static seq_t *dp_prev = NULL;         /* previous distribution */
    static size_t dpSize = 0;             /* number of states in dp_prev */
    static idx_t dpL = 0;                 /* lower bound of partial sums in dp_prev */

    if (n == 0) return 0;                 /* undefined for n < 1, harmless */

    /* Compute terms up to n if necessary */
    if (maxComputed < n) {
        for (idx_t k = maxComputed + 1; k <= n; ++k) {
            /* bounds for partial sum S_k */
            idx_t t   = isqrt_u64(15 * k);                       /* floor sqrt(15*k) */
            idx_t val3 = 3 * k;                                 /* 3*k */
            idx_t L = (val3 - t + 1) / 2;                       /* ceil((3k - t)/2) */
            idx_t U = (val3 + t) / 2;                           /* floor((3k + t)/2) */
            size_t sz = (size_t)(U - L + 1);

            /* new distribution for length k */
            seq_t *dp_next = (seq_t *)calloc(sz, sizeof(seq_t));
            if (!dp_next) return 0;                           /* allocation failure */

            if (k == 1) {
                /* For length 1, each value 0..3 yields one array */
                for (idx_t s = 0; s <= 3; ++s)
                    dp_next[s - L] = 1;
            } else {
                /* Transition from length k-1 to k */
                for (size_t idx = 0; idx < dpSize; ++idx) {
                    seq_t val = dp_prev[idx];
                    if (val == 0) continue;
                    idx_t s_prev = dpL + (idx_t)idx;
                    for (idx_t d = 0; d <= 3; ++d) {
                        idx_t s_new = s_prev + d;
                        if (s_new < L || s_new > U) continue;
                        dp_next[s_new - L] += val;
                    }
                }
            }

            /* Sum all counts to obtain a(k) */
            seq_t total = 0;
            for (size_t idx = 0; idx < sz; ++idx)
                total += dp_next[idx];

            /* Store a(k) */
            if (aSize < (size_t)k) {
                seq_t *tmp = (seq_t *)realloc(a_vals, k * sizeof(seq_t));
                if (!tmp) return 0;
                a_vals = tmp;
                aSize = k;
            }
            a_vals[k - 1] = total;

            /* Prepare for next iteration */
            free(dp_prev);
            dp_prev = dp_next;
            dpSize = sz;
            dpL = L;
        }
        maxComputed = n;
    }

    return a_vals[n - 1];
}
