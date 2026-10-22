/*
 * A351275  a(n) = Sum_{k=0..n} (-2*k)^k * Stirling1(n,k).
 * https://oeis.org/A351275
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} (-2*k)^k * Stirling1(n,k)
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 7, i64 n <= 13, i128 n <= 16,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_pow.h"   /* seq_pow(seq_t base, idx_t exp) */

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *a_cache = NULL;   /* cached a(0..max_computed) */
    static seq_t *s_arr  = NULL;    /* current Stirling numbers s(n,k) */
    static seq_t *T_arr  = NULL;    /* (-2*k)^k values, k=0..max_computed */

    static size_t a_len  = 0;        /* allocated length of a_cache   */
    static size_t s_len  = 0;        /* allocated length of s_arr     */
    static size_t T_len  = 0;        /* allocated length of T_arr     */
    static idx_t  max_computed = 0;  /* highest n already computed   */
    static bool    inited = false;

    /* initialise on first call */
    if (!inited) {
        inited = true;

        /* allocate for the first element of each array */
        a_len = 1;
        a_cache = (seq_t *)malloc(sizeof(seq_t));
        if (!a_cache) abort();
        a_cache[0] = (seq_t)1;          /* a(0) = 1 */

        s_len = 1;
        s_arr = (seq_t *)malloc(sizeof(seq_t));
        if (!s_arr) abort();
        s_arr[0] = (seq_t)1;            /* s(0,0) = 1 */

        T_len = 1;
        T_arr = (seq_t *)malloc(sizeof(seq_t));
        if (!T_arr) abort();
        T_arr[0] = (seq_t)1;            /* (-2*0)^0 = 1 */

        max_computed = 0;
    }

    /* enlarge arrays to accommodate n if necessary */
    if (n > max_computed) {

        /* grow T_arr up to n */
        if (T_len <= (size_t)n) {
            size_t new_T_len = (size_t)n + 1;
            seq_t *tmp = (seq_t *)realloc(T_arr, new_T_len * sizeof(seq_t));
            if (!tmp) abort();
            T_arr = tmp;
            /* compute missing entries */
            for (size_t k = T_len; k <= (size_t)n; ++k) {
                if (k == 0) {
                    T_arr[0] = (seq_t)1;
                } else {
                    int64_t base_int = -((int64_t)2 * (int64_t)k);
                    seq_t base = (seq_t)base_int;
                    T_arr[k] = seq_pow(base, (idx_t)k);
                }
            }
            T_len = new_T_len;
        }

        /* grow a_cache up to n */
        if (a_len <= (size_t)n) {
            size_t new_a_len = (size_t)n + 1;
            seq_t *tmp = (seq_t *)realloc(a_cache, new_a_len * sizeof(seq_t));
            if (!tmp) abort();
            a_cache = tmp;
            a_len = new_a_len;
        }

        /* grow s_arr up to n */
        if (s_len <= (size_t)n) {
            size_t new_s_len = (size_t)n + 1;
            seq_t *tmp = (seq_t *)realloc(s_arr, new_s_len * sizeof(seq_t));
            if (!tmp) abort();
            s_arr = tmp;
            s_len = new_s_len;
        }

        /* compute values from current max_computed+1 up to n */
        for (idx_t m = max_computed; m < n; ++m) {
            idx_t next_n = m + 1;

            /* update Stirling numbers s(next_n,k) in-place (descending k) */
            for (size_t k = (size_t)next_n; k > 0; --k) {
                seq_t old = s_arr[k];
                s_arr[k] = s_arr[k - 1] - (seq_t)m * old;
            }
            s_arr[0] = (seq_t)0;   /* s(next_n,0) = 0 */

            /* compute a(next_n) = sum_{k=0..next_n} s(next_n,k) * T_k */
            seq_t new_a = (seq_t)0;
            for (size_t k = 0; k <= (size_t)next_n; ++k) {
                new_a += s_arr[k] * T_arr[k];
            }
            a_cache[next_n] = new_a;
        }

        max_computed = n;
    }

    return a_cache[n];
}
