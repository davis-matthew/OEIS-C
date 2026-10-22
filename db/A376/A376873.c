/*
 * A376873  a(n) = n! * |Stirling1(2*n, n)|.
 * https://oeis.org/A376873
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * |Stirling1(2*n, n)|
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A376873: a(n) = n! * |Stirling1(2*n, n)| */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static state for memoisation */
    static seq_t *stirling  = NULL;   /* current row of unsigned Stirling1 numbers */
    static seq_t *factorial = NULL;   /* factorial values 0! … max_n! */
    static seq_t *values    = NULL;   /* cached a(n) values */
    static idx_t  max_n     = 0;       /* maximum n that has been computed */
    static idx_t  fact_len  = 0;       /* maximum factorial computed (last index) */
    static idx_t  max_m     = 0;       /* maximum m (= 2*max_n) that has been processed */

    /* initialise state on first call */
    if (max_n == 0 && max_m == 0) {
        /* allocate minimal size 1 for all arrays */
        stirling  = (seq_t *)realloc(stirling,  sizeof(seq_t));
        factorial = (seq_t *)realloc(factorial, sizeof(seq_t));
        values    = (seq_t *)realloc(values,    sizeof(seq_t));
        if (!stirling || !factorial || !values) return 0;

        /* base values */
        stirling[0]  = 1;            /* Stirling1(0,0) = 1 */
        factorial[0] = 1;            /* 0! = 1 */
        values[0]    = 1;            /* a(0) = 0! * |S(0,0)| = 1 */

        max_n    = 0;
        fact_len = 0;
        max_m    = 0;
    }

    /* expand state if a larger n is requested */
    if (n > max_n) {
        idx_t new_max_n = n;
        idx_t new_max_m = 2 * new_max_n;

        /* enlarge Stirling array to new_max_m+1 */
        seq_t *tmp = (seq_t *)realloc(stirling, (new_max_m + 1) * sizeof(seq_t));
        if (!tmp) return 0;
        stirling = tmp;

        /* enlarge factorial array to new_max_n+1 */
        tmp = (seq_t *)realloc(factorial, (new_max_n + 1) * sizeof(seq_t));
        if (!tmp) return 0;
        factorial = tmp;

        /* enlarge values array to new_max_n+1 */
        tmp = (seq_t *)realloc(values, (new_max_n + 1) * sizeof(seq_t));
        if (!tmp) return 0;
        values = tmp;

        /* initialise new entries */
        for (idx_t i = max_n + 1; i <= new_max_n; ++i) {
            values[i] = 0;
        }
        for (idx_t i = max_m + 1; i <= new_max_m; ++i) {
            stirling[i] = 0;
        }

        /* compute Stirling numbers for new m values */
        for (idx_t m = max_m + 1; m <= new_max_m; ++m) {
            /* recurrence: s(m,k) = s(m-1,k-1) + (m-1)*s(m-1,k) */
            for (idx_t k = m; k >= 1; --k) {
                stirling[k] = stirling[k - 1] + (seq_t)(m - 1) * stirling[k];
            }
            stirling[0] = 0;        /* s(m,0) = 0 for m>0 */

            /* if m is even, compute a(k) where k = m/2 */
            if ((m & 1U) == 0) {
                idx_t k = m >> 1;  /* k = m/2 == n for this m */
                /* ensure factorial up to k is computed */
                while (fact_len < k) {
                    ++fact_len;
                    factorial[fact_len] = factorial[fact_len - 1] * (seq_t)fact_len;
                }
                values[k] = factorial[k] * stirling[k];
            }
        }

        /* update limits */
        max_n = new_max_n;
        max_m = new_max_m;
    }

    return values[n];
}
