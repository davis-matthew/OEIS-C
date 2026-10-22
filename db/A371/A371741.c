/*
 * A371741  Triangle read by rows: g.f. (1 - t)^(-x) * (1 + t)^(3-x).
 * https://oeis.org/A371741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = triangle read by rows of (1 - t)^(-x)*(1 + t)^(3-x)
 * Range:     u8 n <= 42, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Forward declaration for external linkage */
seq_t a(idx_t n);

/* Static cache for sequence values */
static seq_t *cache = NULL;
static size_t cache_len   = 0;
static size_t cache_cap   = 0;

/* Stirling numbers of the first kind (unsigned) for the current and previous m */
static seq_t *s_prev = NULL;   /* S1(m-1, k)   */
static seq_t *s_curr = NULL;   /* S1(m,   k)   */
static idx_t   m_curr = 0;     /* current m of s_curr */

/* current processed row number */
static idx_t current_row = 0;

/* Ensure cache and Stirling tables are initialized */
static void init_cache(void)
{
    if (cache == NULL) {
        cache_cap = 1024;
        cache    = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }
    if (s_prev == NULL) {
        s_prev = (seq_t *)malloc(sizeof(seq_t));
        s_prev[0] = (seq_t)1;
    }
    if (s_curr == NULL) {
        s_curr = (seq_t *)malloc(sizeof(seq_t));
        s_curr[0] = (seq_t)1;
    }
    m_curr       = 0;
    current_row = 0;
    cache_len   = 0;
}

/* Ensure cache has at least n+1 entries */
static void ensure_cache_for_index(idx_t n)
{
    if (cache == NULL) init_cache();

    while (cache_len <= n) {
        if (current_row == 0) {
            /* Row 0: single value */
            if (cache_len >= cache_cap) {
                cache_cap <<= 1;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)1;
        } else if (current_row == 1) {
            /* Row 1: single value */
            if (cache_len >= cache_cap) {
                cache_cap <<= 1;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)3;
        } else {
            /* Row >= 2 */
            idx_t m = current_row / 2;   /* floor division */

            /* Build Stirling row for m if necessary */
            while (m_curr < m) {
                idx_t next_m = m_curr + 1;
                seq_t *newS = (seq_t *)malloc((next_m + 1) * sizeof(seq_t));

                for (idx_t k = 0; k <= next_m; ++k) {
                    seq_t a = (k == 0) ? (seq_t)0 : s_curr[k - 1];
                    seq_t b = (k < (m_curr + 1)) ? s_curr[k] : (seq_t)0;
                    newS[k] = a + (seq_t)(next_m - 1) * b;
                }
                /* rotate */
                free(s_prev);
                s_prev = s_curr;
                s_curr = newS;
                m_curr = next_m;
            }

            /* Now generate the current row */
            size_t len_row = (size_t)(m + 1);
            if (cache_len + len_row > cache_cap) {
                while (cache_len + len_row > cache_cap)
                    cache_cap <<= 1;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }

            for (idx_t k = 0; k <= m; ++k) {
                seq_t s1_mk = s_curr[k];
                seq_t s1_m1k = (k < m) ? s_prev[k] : (seq_t)0;
                seq_t val;

                if ((current_row & 1) == 0) {   /* even row: 2*m  */
                    val = s1_mk + (seq_t)(3 * m) * s1_m1k;
                } else {                       /* odd  row: 2*m+1 */
                    val = (seq_t)3 * s1_mk + (seq_t)m * s1_m1k;
                }
                cache[cache_len++] = val;
            }
        }
        ++current_row;
    }
}

/* External function required by the framework */
seq_t a(idx_t n)
{
    ensure_cache_for_index(n);
    return cache[(size_t)n];
}
