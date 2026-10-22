/*
 * A308850  Expansion of e.g.f. exp(-2*x) / (BesselI(0,2*x) + BesselI(1,2*x)).
 * https://oeis.org/A308850
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=1 and for n≥1 * a(n) = –∑_{k=0}^{n‑1} C(n,k)·a(k)·B(n‑k) *
 *            where B(m)=C(2m+1, m+1). This follows from the egf *
 *            A(x)=1/∑_{k≥0} C(2k+1,k+1)·x^k/k!
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 12, i64 n <= 23, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    size_t N = (size_t)n;

#ifdef __cplusplus
    /* C++ implementation using std::vector */
    std::vector<seq_t> a_vec(N + 1);
    std::vector<seq_t> b_vec(N + 1);

    a_vec[0] = (seq_t)1;
    b_vec[0] = (seq_t)1;          /* B(0)=C(1,1)=1 */

    for (size_t m = 1; m <= N; ++m) {
        seq_t num = (seq_t)(2 * m + 1) * (seq_t)(2 * m);
        seq_t den = (seq_t)m * (seq_t)(m + 1);
        b_vec[m] = (b_vec[m - 1] * num) / den;
    }

    for (size_t i = 1; i <= N; ++i) {
        seq_t sum  = 0;
        seq_t binom = 1;          /* C(i,0) */
        for (size_t k = 0; k < i; ++k) {
            sum += binom * a_vec[k] * b_vec[i - k];
            /* update binom to C(i,k+1) */
            binom = (binom * (seq_t)(i - k)) / (seq_t)(k + 1);
        }
        a_vec[i] = -sum;
    }

    return a_vec[N];

#else
    /* C implementation using malloc/free */
    seq_t *a_arr = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    seq_t *b_arr = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (!a_arr || !b_arr) {        /* allocation failure */
        free(a_arr);
        free(b_arr);
        return (seq_t)0;
    }

    a_arr[0] = (seq_t)1;
    b_arr[0] = (seq_t)1;          /* B(0)=1 */

    for (size_t m = 1; m <= N; ++m) {
        seq_t num = (seq_t)(2 * m + 1) * (seq_t)(2 * m);
        seq_t den = (seq_t)m * (seq_t)(m + 1);
        b_arr[m] = (b_arr[m - 1] * num) / den;
    }

    for (size_t i = 1; i <= N; ++i) {
        seq_t sum  = 0;
        seq_t binom = 1;          /* C(i,0) */
        for (size_t k = 0; k < i; ++k) {
            sum += binom * a_arr[k] * b_arr[i - k];
            /* update binom to C(i,k+1) */
            binom = (binom * (seq_t)(i - k)) / (seq_t)(k + 1);
        }
        a_arr[i] = -sum;
    }

    seq_t result = a_arr[N];
    free(a_arr);
    free(b_arr);
    return result;
#endif
}
