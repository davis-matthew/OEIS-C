/*
 * A348087  a(n) = [x^n] Product_{k=1..n} 1/(1 - (2*k-1) * x).
 * https://oeis.org/A348087
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k=1..n} 1/(1-(2k-1)x)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a_n(0)=1,  a_n(i)=a_{n-1}(i)+(2n-1)*a_n(i-1) for i≥1  */
#ifndef __KERNEL__
#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    size_t len = (size_t)n + 1;            /* array length needed for indices 0..n */

#ifdef __cplusplus
    /* Use std::vector for C++ builds to construct arbitrary‑precision objects */
    std::vector<seq_t> buf1(len, seq_t());
    std::vector<seq_t> buf2(len, seq_t());
    seq_t *a_arr = buf1.data();
    seq_t *cur_arr = buf2.data();
#else
    /* Use malloc/calloc for plain C builds (seq_t is a primitive integer type) */
    seq_t *a_arr  = (seq_t*)calloc(len, sizeof(seq_t));
    seq_t *cur_arr = (seq_t*)calloc(len, sizeof(seq_t));
    if (!a_arr || !cur_arr) return (seq_t)0;      /* allocation failure: return 0 */
#endif

    /* initialise a_0(x) = 1 */
    a_arr[0] = (seq_t)1;

    for (idx_t k = 1; k <= n; ++k) {
        seq_t c = (seq_t)((idx_t)2 * k - 1);   /* 2k-1 as seq_t */
        cur_arr[0] = a_arr[0];                 /* a_n(0) = a_{n-1}(0) = 1 */

        /* compute a_n(i) for i = 1 .. n using recurrence */
        for (idx_t i = 1; i <= n; ++i) {
            cur_arr[i] = a_arr[i] + c * cur_arr[i - 1];
        }

        /* swap pointers for next iteration */
        seq_t *tmp = a_arr;
        a_arr = cur_arr;
        cur_arr = tmp;
    }

    seq_t result = a_arr[n];                    /* a(n) = coefficient of x^n */

#ifndef __cplusplus
    free(a_arr);
    free(cur_arr);
#endif
    return result;
}
#endif
