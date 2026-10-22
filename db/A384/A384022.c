/*
 * A384022  a(n) = [x^(2*n)] Product_{k=0..n} 1/(1 - k*x)^3.
 * https://oeis.org/A384022
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A384022: a(n) = [x^(2*n)] Product_{k=0..n} 1/(1 - k*x)^3 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* trivial base */
    if (n == 0) return (seq_t)1;

    /* n may be up to 10000; everything fits in size_t for array sizes */
    size_t N = (size_t)n;
    size_t M  = 3 * N;          /* highest m needed: 3n  */
    size_t Slen = 2 * N + 1;    /* length of the s[] array */

    /* allocate arrays for two consecutive columns of Stirling numbers */
#ifdef __cplusplus
    seq_t *prev = new seq_t[M + 1];
    seq_t *cur  = new seq_t[M + 1];
#else
    seq_t *prev = (seq_t *)malloc((M + 1) * sizeof(seq_t));
    seq_t *cur  = (seq_t *)malloc((M + 1) * sizeof(seq_t));
#endif
    if (!prev || !cur) {
#ifdef __cplusplus
        delete[] prev;
        delete[] cur;
#else
        free(prev);
        free(cur);
#endif
        return (seq_t)0;  /* allocation failure */
    }

    /* initialise column 0 : S(0,0)=1, others 0 */
    for (size_t i = 0; i <= M; ++i) prev[i] = (seq_t)0;
    prev[0] = (seq_t)1;

    /* build columns 1..N */
    for (size_t nn = 1; nn <= N; ++nn) {
        cur[0] = (seq_t)0;          /* S(0,nn)=0 for nn>0 */
        seq_t nn_seq = (seq_t)nn;
        for (size_t m = 1; m <= M; ++m) {
            /* S(m,nn) = nn * S(m-1,nn) + S(m-1,nn-1) */
            cur[m] = prev[m - 1] + nn_seq * cur[m - 1];
        }
        /* swap pointers for next iteration */
        seq_t *tmp = prev;
        prev = cur;
        cur = tmp;
    }

    /* extract the values s[i] = S(i+N,N) for i=0..2N */
#ifdef __cplusplus
    seq_t *s = new seq_t[Slen];
#else
    seq_t *s = (seq_t *)malloc(Slen * sizeof(seq_t));
#endif
    if (!s) {
#ifdef __cplusplus
        delete[] prev;
        delete[] cur;
        delete[] s;
#else
        free(prev);
        free(cur);
        free(s);
#endif
        return (seq_t)0;
    }
    for (size_t i = 0; i < Slen; ++i) s[i] = prev[i + N];

    /* compute the triple sum: a = sum_{i,j,k >= 0, i+j+k=2N} s[i]*s[j]*s[k] */
    seq_t a_val = (seq_t)0;
    size_t TwoN = 2 * N;
    for (size_t i = 0; i <= TwoN; ++i) {
        seq_t si = s[i];
        size_t maxj = TwoN - i;
        for (size_t j = 0; j <= maxj; ++j) {
            seq_t prod = si * s[j];
            prod *= s[TwoN - i - j];
            a_val += prod;
        }
    }

    /* free temporary arrays */
#ifdef __cplusplus
    delete[] prev;
    delete[] cur;
    delete[] s;
#else
    free(prev);
    free(cur);
    free(s);
#endif

    return a_val;
}
