/*
 * A398654  Upper (1, 1/3) midsequence of triangular numbers (A000217) and
 *          tetrahedral numbers (A000330); see Comments.
 * https://oeis.org/A398654
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 4*a(n-1) - 6*a(n-2) + 3*a(n-3) + 3*a(n-4) 
 *              - 6*a(n-5) + 3*a(n-6) + 3*a(n-7) - 6*a(n-8) 
 *              + 4*a(n-9) - a(n-10)
 * with (a(0), ..., a(9)) = (0, 2, 5, 11, 20, 34, 52, 75, 104, 140)
 */

#include <stddef.h>
#ifdef __cplusplus
#include <cstdlib>
#else
#include <stdlib.h>
#endif

seq_t a(idx_t n)
{
    static seq_t *buf = NULL;
    static size_t buf_len = 0;
    static idx_t max_computed = 9;   /* last index that has been filled */

    /* First call: allocate and initialise the first 10 values */
    if (buf == NULL) {
        buf_len = 16;
        buf = (seq_t*)calloc(buf_len, sizeof(seq_t));
        if (!buf) return 0;         /* allocation failed – return 0 */
        const seq_t init[10] = {0, 2, 5, 11, 20, 34, 52, 75, 104, 140};
        for (size_t i = 0; i < 10; ++i) buf[i] = init[i];
    }

    /* Grow buffer if needed */
    if ((size_t)n >= buf_len) {
        size_t newlen = buf_len;
        while ((size_t)n >= newlen) newlen <<= 1;
        seq_t *tmp = (seq_t*)realloc(buf, newlen * sizeof(seq_t));
        if (!tmp) return 0;         /* allocation failed */
        /* initialise new elements to zero */
        for (size_t i = buf_len; i < newlen; ++i) tmp[i] = 0;
        buf = tmp;
        buf_len = newlen;
    }

    /* Compute any missing terms up to n */
    if (n > max_computed) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            /* use unsigned long long when compiling as C, because
               seq_t may be a small builtin type. */
            #ifdef __cplusplus
            seq_t pos = (seq_t)4 * buf[(size_t)(i-1)]
                      + (seq_t)3 * buf[(size_t)(i-3)]
                      + (seq_t)3 * buf[(size_t)(i-4)]
                      + (seq_t)3 * buf[(size_t)(i-6)]
                      + (seq_t)3 * buf[(size_t)(i-7)]
                      + (seq_t)4 * buf[(size_t)(i-9)];
            seq_t neg = (seq_t)6 * buf[(size_t)(i-2)]
                      + (seq_t)6 * buf[(size_t)(i-5)]
                      + (seq_t)6 * buf[(size_t)(i-8)]
                      +          buf[(size_t)(i-10)];
            #else
            unsigned long long pos = 4ULL * (unsigned long long)buf[(size_t)(i-1)]
                                   + 3ULL * (unsigned long long)buf[(size_t)(i-3)]
                                   + 3ULL * (unsigned long long)buf[(size_t)(i-4)]
                                   + 3ULL * (unsigned long long)buf[(size_t)(i-6)]
                                   + 3ULL * (unsigned long long)buf[(size_t)(i-7)]
                                   + 4ULL * (unsigned long long)buf[(size_t)(i-9)];
            unsigned long long neg = 6ULL * (unsigned long long)buf[(size_t)(i-2)]
                                   + 6ULL * (unsigned long long)buf[(size_t)(i-5)]
                                   + 6ULL * (unsigned long long)buf[(size_t)(i-8)]
                                   +          (unsigned long long)buf[(size_t)(i-10)];
            #endif

            buf[(size_t)i] = pos - neg;   /* result fits in seq_t for the tested range */
        }
        max_computed = n;
    }

    return buf[(size_t)n];
}
