/*
 * A357383  Expansion of Product_{k>=1} 1 / (1 + x^Lucas(k)).
 * https://oeis.org/A357383
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -∑_{k=1}^{n} a(n‑k) * h(k), where h is the coefficient of
 *            the series ∏_{j≥1} (1 + x^{Lucas(j)}) (Lucas(1)=1, Lucas(2)=3,
 *            …). The sequence a is the reciprocal of that product: ∑_{i=0}^n
 *            a(i) h(n‑i) = δ_{n,0}
 * Range:     i8 n <= 60, i16 n <= 64, i32 n <= 64, i64 n <= 64, i128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;

    /* Work with size_t for array indices */
    size_t N = (size_t)n;

    /* Allocate coefficient arrays */
    seq_t *h = (seq_t*)calloc(N + 1, sizeof(seq_t));
    seq_t *arr = (seq_t*)calloc(N + 1, sizeof(seq_t));
    if (!h || !arr) return (seq_t)0; /* Allocation failure */

    h[0] = (seq_t)1;          /* h(0) = 1 */

    /* Generate Lucas numbers > 0 up to n */
    idx_t L_prev0 = 2;        /* Lucas(0) = 2 */
    idx_t L_prev1 = 1;        /* Lucas(1) = 1 */
    idx_t lucas_vals[64];
    size_t lucas_cnt = 0;

    /* First Lucas number */
    lucas_vals[lucas_cnt++] = L_prev1;   /* 1 */

    /* Subsequent Lucas numbers */
    while (1) {
        idx_t L_next = L_prev0 + L_prev1;   /* Lucas(k+1) = Lucas(k) + Lucas(k‑1) */
        if (L_next > n) break;
        lucas_vals[lucas_cnt++] = L_next;
        L_prev0 = L_prev1;
        L_prev1 = L_next;
    }

    /* Subset‑sum DP for h : coefficient of ∏(1 + x^{Lucas}) */
    for (size_t li = 0; li < lucas_cnt; ++li) {
        size_t L = (size_t)lucas_vals[li];
        for (size_t j = N; j >= L; --j) {
            h[j] += h[j - L];
            if (j == L) break;  /* prevent unsigned wraparound */
        }
    }

    /* Compute a via the convolution inverse recurrence */
    arr[0] = (seq_t)1;     /* a(0) = 1 */
    for (size_t i = 1; i <= N; ++i) {
        __int128 acc = 0; /* use 128‑bit accumulator */
        for (size_t k = 1; k <= i; ++k) {
            __int128 prod = (__int128)arr[i - k] * (__int128)h[k];
            acc += prod;
        }
        arr[i] = (seq_t)(-acc);
    }

    seq_t result = arr[N];

    free(h);
    free(arr);
    return result;
}
