/*
 * A329967  a(0) = 1; a(n) = Sum_{k=1..n} Stirling1(n,k) * a(n-k).
 * https://oeis.org/A329967
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=1..n} Stirling1(n,k) * a(n-k), a(0) = 1
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 7, i64 n <= 9, i128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

#if defined(__cplusplus)
#define ALLOC_SEQ_T(ptr, sz) ptr = new seq_t[(sz)](); /* value-initialize */
#define FREE_SEQ_T(ptr) delete[] ptr;
#else
#define ALLOC_SEQ_T(ptr, sz) ptr = (seq_t*)malloc(sizeof(seq_t)*(sz)); memset(ptr, 0, sizeof(seq_t)*(sz));
#define FREE_SEQ_T(ptr) free(ptr);
#endif

seq_t a(idx_t n) {
    static seq_t *a_arr = NULL;
    static size_t a_cap = 0;
    static size_t a_len = 0;
    static size_t max_done = 0;
    static seq_t *prev_s = NULL;
    static size_t prev_len = 0;

    size_t N = (size_t)n;

    if (a_arr == NULL) {
        a_cap = 1;
        ALLOC_SEQ_T(a_arr, a_cap);
        a_arr[0] = (seq_t)1;
        a_len = 1;
        max_done = 0;
        prev_s = NULL;
        prev_len = 0;
    }

    if (N > max_done) {
        if (prev_s == NULL) {
            ALLOC_SEQ_T(prev_s, 1);
            prev_s[0] = (seq_t)1;   /* s(0,0)=1 */
            prev_len = 1;
        }

        for (size_t cur = max_done + 1; cur <= N; ++cur) {
            size_t new_len = cur + 1;
            seq_t *curr_s = NULL;
            ALLOC_SEQ_T(curr_s, new_len);

            curr_s[0] = 0;
            for (size_t k = 1; k <= cur; ++k) {
                seq_t term1 = (k-1 < prev_len) ? prev_s[k-1] : (seq_t)0;
                seq_t term2 = (k < prev_len) ? prev_s[k] : (seq_t)0;
                curr_s[k] = term1 - (seq_t)(cur - 1) * term2;
            }

            seq_t res = 0;
            for (size_t k = 1; k <= cur; ++k) {
                res += curr_s[k] * a_arr[cur - k];
            }

            if (cur >= a_cap) {
                size_t new_cap = a_cap * 2;
                if (new_cap == 0) new_cap = 4;
                while (new_cap <= cur) new_cap *= 2;
                seq_t *tmp = NULL;
                ALLOC_SEQ_T(tmp, new_cap);
                for (size_t i = 0; i < a_len; ++i) tmp[i] = a_arr[i];
                FREE_SEQ_T(a_arr);
                a_arr = tmp;
                a_cap = new_cap;
            }

            a_arr[cur] = res;
            if (cur + 1 > a_len) a_len = cur + 1;

            FREE_SEQ_T(prev_s);
            prev_s = curr_s;
            prev_len = new_len;
        }

        max_done = N;
    }

    return a_arr[N];
}
