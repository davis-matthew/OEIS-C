/*
 * A377046  Array read by downward antidiagonals where A(n,k) is the n-th term
 *          of the k-th differences of nonsquarefree numbers.
 * https://oeis.org/A377046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = value of A(n,k) where A(i,j) = Σ_{t=0..j} (-1)^(j-t)
 *            C(j,t) A013929(i+t) A013929 is the sequence of nonsquarefree
 *            numbers. The array A is read by downward antidiagonals
 * Range:     i8 n <= 64, i16 n <= 77, i32 n <= 77, i64 n <= 77, i128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

/* static cache for the base sequence A013929 */
static seq_t *base_seq = NULL;   /* stores the nonsquarefree numbers */
static idx_t  base_len = 0;      /* number of elements stored (base_len == needed+1) */

/* generate the first (need+1) terms of A013929 (0‑based) */
static void ensure_base(idx_t need)
{
    if (need + 1 <= base_len) return;          /* already have enough */

    seq_t *new_seq = (seq_t*)malloc((need + 1) * sizeof(seq_t));
    idx_t count  = 0;
    uint64_t num = 4;                          /* first nonsquarefree number */

    while (count <= need) {
        bool nonsq = false;
        for (uint64_t p = 2; p * p <= num; ++p) {
            if (num % (p * p) == 0) { nonsq = true; break; }
        }
        if (nonsq) {
            new_seq[count] = (seq_t)num;
            ++count;
        }
        ++num;
    }

    free(base_seq);
    base_seq = new_seq;
    base_len = need + 1;
}

/* compute A(i,j) = Σ_{t=0..j} (-1)^(j-t) C(j,t) base_seq[i+t] */
static seq_t compute_Aij(idx_t i, idx_t j)
{
    seq_t val   = 0;
    seq_t binom = 1;   /* C(j,0) */

    for (idx_t t = 0; t <= j; ++t) {
        if (t > 0) {
            /* binom = binom * (j-t+1) / t  (integer division is exact) */
            binom = (binom * (seq_t)(j - (t - 1))) / (seq_t)t;
        }

        seq_t term = base_seq[i + t];
        if ((j - t) & 1) term = -term;          /* apply sign */
        val += term * binom;
    }
    return val;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* find s such that s(s+1)/2 <= n < (s+1)(s+2)/2 */
    double temp = sqrt(8.0 * (double)n + 1.0);
    idx_t   s   = (idx_t)((temp - 1.0) / 2.0);
    /* adjust for possible rounding errors */
    while ((s + 1) * (s + 2) / 2 <= n) ++s;
    while (s * (s + 1) / 2 > n) --s;

    idx_t d = n - s * (s + 1) / 2;     /* offset in the s‑th diagonal */
    idx_t i = s - d;                   /* row index (k) */
    idx_t j = d;                       /* column index (d) */

    /* need base terms up to index i+j (<= s) */
    ensure_base(i + j);

    return compute_Aij(i, j);
}
