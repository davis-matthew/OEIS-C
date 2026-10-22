/*
 * A363424  G.f. satisfies A(x) = exp( Sum_{k>=1} A(4*x^k) * x^k/k ).
 * https://oeis.org/A363424
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(0) = 1
 * a(n) = (1/n) * Σ_{k=1..n} ( Σ_{d|k} d * 4^(d-1) * a(d-1) ) * a(n-k)
 */
#include <stdbool.h>
#include <stdlib.h>   /* for malloc, realloc, free */

seq_t a(idx_t n)
{
    /* static buffers for memoisation */
    static seq_t *a_buf  = NULL;   /* a(0 .. max_cached)           */
    static seq_t *s_buf  = NULL;   /* S(k) = Σ_{d|k} d*4^(d-1)*a(d-1) for k<=max_cached */
    static seq_t *pow4   = NULL;   /* 4^k for k up to max_cached   */
    static size_t buf_cap = 0;     /* allocated size of arrays       */
    static size_t max_cached = 0;  /* largest n that has been computed */

    /* enlarge buffers if needed */
    if (buf_cap <= (size_t)n) {
        size_t new_cap = buf_cap ? buf_cap << 1 : 2;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;

        a_buf  = (seq_t *)realloc(a_buf,  new_cap * sizeof(seq_t));
        s_buf  = (seq_t *)realloc(s_buf,  new_cap * sizeof(seq_t));
        pow4   = (seq_t *)realloc(pow4,   new_cap * sizeof(seq_t));

        /* initialise the newly allocated part to zero */
        for (size_t i = buf_cap; i < new_cap; ++i) {
            a_buf[i] = (seq_t)0;
            s_buf[i] = (seq_t)0;
            pow4[i]  = (seq_t)0;
        }
        buf_cap = new_cap;
    }

    /* initialise a(0) once */
    if (max_cached == 0)
        a_buf[0] = (seq_t)1;   /* a(0) = 1 */

    /* compute powers of 4 up to n */
    static size_t pow4_len = 0;  /* number of powers already computed   */
    if (pow4_len <= (size_t)n) {
        if (pow4_len == 0) {          /* initialise 4^0 */
            pow4[0] = (seq_t)1;
            pow4_len = 1;
        }
        for (size_t i = pow4_len; i <= (size_t)n; ++i)
            pow4[i] = pow4[i-1] * (seq_t)4;
        pow4_len = (size_t)n + 1;
    }

    /* return cached value if already known */
    if ((size_t)n <= max_cached)
        return a_buf[(size_t)n];

    /* compute missing terms up to n */
    for (size_t m = max_cached + 1; m <= (size_t)n; ++m) {
        /* --- compute S(m) = Σ_{d|m} d * 4^(d-1) * a(d-1) --- */
        seq_t s_val = (seq_t)0;
        for (size_t d = 1; d * d <= m; ++d) {
            if (m % d == 0) {
                /* divisor d */
                idx_t d1 = (idx_t)d;
                s_val += d1 * pow4[d1-1] * a_buf[d1-1];

                /* paired divisor m/d, if distinct */
                idx_t d2 = (idx_t)(m / d);
                if (d2 != d1) {
                    s_val += d2 * pow4[d2-1] * a_buf[d2-1];
                }
            }
        }
        s_buf[m] = s_val;

        /* --- compute a(m) = (1/m) * Σ_{k=1..m} S(k) * a(m-k) --- */
        seq_t a_val = (seq_t)0;
        for (size_t k = 1; k <= m; ++k) {
            a_val += s_buf[k] * a_buf[m - k];
        }
        a_buf[m] = a_val / (seq_t)m;
    }

    max_cached = (size_t)n;
    return a_buf[(size_t)n];
}
