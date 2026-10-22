/*
 * A371328  E.g.f. satisfies A(x) = -log(1 - x/(1 - A(x)))/(1 - A(x))^2.
 * https://oeis.org/A371328
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (n+3*k-2)!/(n+2*k-1)! * |Stirling1(n,k)|
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function memoises computed values and Stirling‑1 rows for efficiency. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache for sequence values a(0)...a(A_len-1) */
    static seq_t *A = NULL;
    static size_t A_cap = 0, A_len = 0;

    /* static buffers for two consecutive rows of unsigned Stirling numbers */
    static seq_t *prev = NULL, *cur = NULL;
    static size_t prev_len = 0, arrays_cap = 0;

    /* ----------------------------------------------------------------- */
    /* initialise the caches on first entry                               */
    /* ----------------------------------------------------------------- */
    if (A_len == 0) {
        /* sequence cache */
        A_cap  = 16;
        A      = (seq_t *)malloc(A_cap * sizeof(seq_t));
        if (!A) return (seq_t)0;
        A[0] = (seq_t)0;           /* a(0) = 0 */
        A_len = 1;

        /* Stirling row for n = 0 : s(0,0) = 1 */
        arrays_cap = 1;
        prev = (seq_t *)malloc(arrays_cap * sizeof(seq_t));
        cur  = (seq_t *)malloc(arrays_cap * sizeof(seq_t));
        if (!prev || !cur) return (seq_t)0;
        prev[0] = (seq_t)1;
        prev_len = 1;
    }

    /* ----------------------------------------------------------------- */
    /* extend caches up to requested n                                 */
    /* ----------------------------------------------------------------- */
    if ((idx_t)n >= A_len) {
        /* Ensure capacity for sequence cache */
        if ((size_t)n >= A_cap) {
            size_t new_cap = A_cap ? A_cap << 1 : 16;
            while (new_cap <= (size_t)n) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;
            A = tmp;  A_cap = new_cap;
        }

        /* Ensure capacity for Stirling buffers */
        if ((size_t)n >= arrays_cap) {
            size_t new_cap = arrays_cap ? arrays_cap << 1 : 16;
            while (new_cap <= (size_t)n) new_cap <<= 1;
            seq_t *tmp  = (seq_t *)realloc(prev, new_cap * sizeof(seq_t));
            seq_t *tmp2 = (seq_t *)realloc(cur , new_cap * sizeof(seq_t));
            if (!tmp || !tmp2) return (seq_t)0;
            prev = tmp;  cur = tmp2;  arrays_cap = new_cap;
        }

        /* compute rows from current A_len to n */
        for (idx_t m = A_len; m <= n; ++m) {
            /* --------------------------------------------------------------------- */
            /* compute Stirling numbers of the first kind for row m (unsigned signed) */
            /* --------------------------------------------------------------------- */
            cur[0] = (seq_t)0;
            for (idx_t k = 1; k <= m; ++k) {
                seq_t part1 = (k-1 < prev_len) ? prev[k-1] : (seq_t)0;
                seq_t part2 = (k < prev_len)     ? (seq_t)(m-1) * prev[k] : (seq_t)0;
                cur[k] = part1 + part2;
            }

            /* --------------------------------------------------------------------- */
            /* compute a(m) = Sum_{k=1..m} ratio_k * cur[k]                          */
            /* ratio_k = (m+3k-2)!/(m+2k-1)! = product_{t=1}^{k-1} (m+2k-1 + t)      */
            /* --------------------------------------------------------------------- */
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; k <= m; ++k) {
                /* compute ratio on the fly */
                seq_t ratio = (seq_t)1;
                for (idx_t t = 1; t < k; ++t) {
                    ratio *= (seq_t)(m + 2*k - 1 + t);
                }
                sum += ratio * cur[k];
            }
            A[m] = sum;

            /* swap rows for next iteration */
            seq_t *tmp = prev;
            prev = cur;
            cur  = tmp;
            prev_len = m + 1;
        }

        A_len = n + 1;
    }

    /* ----------------------------------------------------------------- */
    /* return cached value                                               */
    /* ----------------------------------------------------------------- */
    return A[n];
}
