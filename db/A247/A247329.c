/*
 * A247329  a(n) = Sum_{k=0..n} (-1)^(n-k) * C(n,k) * Stirling1(n+1, k+1).
 * https://oeis.org/A247329
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = \sum_{k=0..n} (-1)^{n-k} \binom{n}{k}\,Stirling1(n+1,k+1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Using the fact that Stirling1 is signed:  St1(n+1,k+1)=(-1)^{n-k}|c(n+1,k+1) */
/* Hence a(n) = sum_{k=0..n} C(n,k) * |c(n+1,k+1)|, where c(.) are unsigned Stirling numbers of the first kind */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for previously computed terms */
    static seq_t *cache      = NULL; /* a values */
    static size_t cache_cap  = 0;
    static size_t cache_len  = 0;     /* number of terms stored (= max index+1) */

    /* Static working array for unsigned Stirling numbers c(n,k) */
    static seq_t *c          = NULL; /* triangular array with indices 0..current_row+1 */
    static size_t c_cap      = 0;     /* capacity of the array */
    static idx_t  current_row = 0;    /* the index of the current row stored in c   */
                                      /* after init, c represents row 0 with c[0]=1   */

    /* Initialisation */
    if (cache_len == 0) {
        /* create minimal c array of capacity 2 (indices 0 and 1)  */
        c_cap = 4;
        c = (seq_t*)malloc(c_cap * sizeof(seq_t));
        c[0] = 1;          /* row 0: only c[0] = 1 */
        current_row = 0;    /* current_row == 0  (row 0)     */
        cache_cap = 8;
        cache    = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
    }

    /* Ensure cache contains a(n) */
    if (n >= cache_len) {
        /* Grow cache buffer if needed */
        if (n + 1 > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 8;
            while (new_cap <= n + 1) new_cap *= 2;
            cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }

        /* Extend computation up to n */
        while (cache_len <= n) {
            /* Make sure we have the Stirling row needed:
               we need c to represent row current_row,
               but we need row (cache_len+1) to compute a(cache_len).
               If current_row < cache_len+1, generate next rows.               */
            while (current_row <= cache_len) {
                idx_t m = current_row;          /* old row index                */
                /* Ensure c has space for index m+1           */
                if (c_cap <= m + 1) {
                    size_t new_c_cap = c_cap ? c_cap * 2 : 4;
                    while (new_c_cap <= m + 2) new_c_cap *= 2;
                    c = (seq_t*)realloc(c, new_c_cap * sizeof(seq_t));
                    c_cap = new_c_cap;
                }
                seq_t old_m = c[m];
                /* In-place construction of next row (length m+2) */
                for (idx_t k = m; k >= 1; --k) {
                    seq_t old_k    = c[k];
                    seq_t old_sp1  = c[k - 1];
                    c[k] = old_sp1 + (seq_t)m * old_k;
                }
                c[m + 1] = old_m;   /* new element at end             */
                c[0] = 0;           /* new row starts with 0           */
                current_row++;      /* now c represents row new current_row   */
            }

            /* Now current_row == cache_len + 1 */
            idx_t m = cache_len;            /* n value for which we compute a(m) */
            seq_t sum = 0;
            seq_t bin = 1;                   /* binomial coefficient \binom{m}{0} */
            for (idx_t k = 0; k <= m; ++k) {
                sum += bin * c[k + 1];      /* c[k+1] is |c(m+1,k+1)|          */
                if (k < m) {
                    bin = bin * (seq_t)(m - k) / (seq_t)(k + 1);
                }
            }
            cache[cache_len] = sum;
            cache_len++;
        }
    }
    return cache[n];
}
