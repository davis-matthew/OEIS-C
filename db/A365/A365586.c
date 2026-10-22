/*
 * A365586  Expansion of e.g.f. 1 / (1 + 5 * log(1-x))^(3/5).
 * https://oeis.org/A365586
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (5*n - 2*k) * n! / (k * (n-k)! * n) * a(n-k)
 *            with a(0) = 1 This recurrence is equivalent to the one given in
 *            the OEIS entry for sequence A365586
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#if defined(__cplusplus)
#include <vector>
#endif

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

#if defined(__cplusplus)
    std::vector<seq_t> a_arr(n + 1);
    std::vector<seq_t> fact_arr(n + 1);
    a_arr[0] = (seq_t)1;
    fact_arr[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        fact_arr[i] = fact_arr[i-1] * (seq_t)i;
    }
    for (idx_t nn = 1; nn <= n; ++nn) {
        seq_t sum = 0;
        seq_t fact_n = fact_arr[nn];
        for (idx_t k = 1; k <= nn; ++k) {
            seq_t t = fact_n / (seq_t)k;
            t = t / fact_arr[nn - k];
            idx_t num = 5*nn - 2*k;
            seq_t term = t * (seq_t)num;
            term = term / (seq_t)nn;
            term = term * a_arr[nn - k];
            sum += term;
        }
        a_arr[nn] = sum;
    }
    return a_arr[n];
#else
    /* In a C build seq_t is a plain unsigned integer type.  We allocate
       temporary arrays with calloc so that they are zero‑initialised. */
    seq_t *a_arr = (seq_t*)calloc((size_t)n + 1, sizeof(seq_t));
    seq_t *fact_arr = (seq_t*)calloc((size_t)n + 1, sizeof(seq_t));
    if (!a_arr || !fact_arr) {
        free(a_arr);
        free(fact_arr);
        return (seq_t)0;   /* allocation failure */
    }
    a_arr[0] = (seq_t)1;
    fact_arr[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        fact_arr[i] = fact_arr[i-1] * (seq_t)i;
    }
    for (idx_t nn = 1; nn <= n; ++nn) {
        seq_t sum = 0;
        seq_t fact_n = fact_arr[nn];
        for (idx_t k = 1; k <= nn; ++k) {
            seq_t t = fact_n / (seq_t)k;
            t = t / fact_arr[nn - k];
            idx_t num = 5*nn - 2*k;
            seq_t term = t * (seq_t)num;
            term = term / (seq_t)nn;
            term = term * a_arr[nn - k];
            sum += term;
        }
        a_arr[nn] = sum;
    }
    seq_t result = a_arr[n];
    free(a_arr);
    free(fact_arr);
    return result;
#endif
}
