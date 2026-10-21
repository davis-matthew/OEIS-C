/*
 * A278039  The tribonacci representation of a(n) is obtained by appending a 0
 *          to the tribonacci representation of n (cf. A278038).
 * https://oeis.org/A278039
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   For n ≥ 0, write the tribonacci (Zeckendorf‑style)
 *            representation * of n as n = Σ d_i * w_i where * w_0 = 1, w_1 =
 *            2, w_2 = 4, * w_k = w_{k‑1} + w_{k‑2} + w_{k‑3} (k ≥ 3), * and
 *            each d_i is 0 or 1. Then * * a(n) = Σ d_i * w_{i+1} * * i.e.
 *            a(n) is obtained by appending a 0 to the tribonacci repres...
 * Range:     u8 n <= 138, u16 n <= 9986, u32 n <= 9986, u64 n <= 9986,
 *            u128 n <= 9986, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*----------------------------------------------------------------------*/
#include <stdlib.h>  /* malloc, realloc */

seq_t a(idx_t n)
{
    /* n = 0 is trivial */
    if (n == 0)
        return 0;

    /* Static weight table, initialized on first call */
    static seq_t *w = NULL;     /* weight array */
    static size_t wlen = 0;     /* number of stored weights */
    static size_t wcap = 0;     /* capacity of the allocation */

    if (w == NULL) {
        wcap = 4;
        w = (seq_t *)malloc(wcap * sizeof(seq_t));
        w[0] = 1;   /* w_0 */
        w[1] = 2;   /* w_1 */
        w[2] = 4;   /* w_2 */
        wlen = 3;   /* we have 0,1,2 indices filled */
    }

    /* Ensure the table contains a weight > n, so that the
     * representation can be built.  One more weight is also produced
     * so that w[i+1] exists for the largest used position i.           */
    while (wlen > 0 && w[wlen-1] <= (seq_t)n) {
        if (wlen + 1 > wcap) {
            wcap = wcap * 2 + 2;
            w = (seq_t *)realloc(w, wcap * sizeof(seq_t));
        }
        w[wlen] = w[wlen-1] + w[wlen-2] + w[wlen-3];
        ++wlen;
    }

    /* Find the largest index i with w[i] ≤ n.              */
    int i = (int)wlen - 1;
    while (i > 0 && w[i] > (seq_t)n)
        --i;

    seq_t rem = (seq_t)n;   /* working copy of n   */
    seq_t res = 0;          /* the answer to accumulate */

    /* Greedy decomposition and simultaneous shift of the weights   */
    for (int j = i; j >= 0; --j) {
        if (rem >= w[j]) {
            rem -= w[j];
            res += w[j+1];   /* shift to the next higher weight   */
        }
    }

    return res;
}
