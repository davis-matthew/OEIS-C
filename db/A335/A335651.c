/*
 * A335651  a(n) is the sum, over all overpartitions of n, of the
 *          non-overlined parts.
 * https://oeis.org/A335651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} σ(k) * OP(n-k) where σ(k) is the sum of
 *            divisors of k and OP(m) is the number of overpartitions of m.
 *            OP(m) is computed as the convolution of ordinary partition
 *            numbers p(j) and distinct-partition numbers d(m-j):
 *            OP(m)=Σ_{j=0..m} p(j)*d(m-j)
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* ---------- internal state ---------- */
static seq_t *sigma_arr = NULL;
static seq_t *p_arr     = NULL;
static seq_t *d_arr     = NULL;
static seq_t *op_arr    = NULL;
static seq_t *a_arr     = NULL;
static idx_t  cur_max   = 0;

/* ---------- helper to recompute up to N ---------- */
static void recompute_upto(idx_t N)
{
    idx_t size = N + 1;
    size_t sz  = (size_t)size;

    /* allocate new arrays */
    seq_t *new_sigma = (seq_t*)malloc(sz * sizeof(seq_t));
    seq_t *new_p     = (seq_t*)malloc(sz * sizeof(seq_t));
    seq_t *new_d     = (seq_t*)malloc(sz * sizeof(seq_t));
    seq_t *new_op    = (seq_t*)malloc(sz * sizeof(seq_t));
    seq_t *new_a     = (seq_t*)malloc(sz * sizeof(seq_t));

    /* zero init */
    for (size_t i = 0; i < sz; ++i) {
        new_sigma[i] = 0;
        new_p[i]     = 0;
        new_d[i]     = 0;
        new_op[i]    = 0;
        new_a[i]     = 0;
    }

    /* 1. σ(k) : sum of divisors */
    for (idx_t i = 1; i <= N; ++i) {
        seq_t val = (seq_t)i;
        for (idx_t j = i; j <= N; j += i) {
            new_sigma[j] += val;
        }
    }

    /* 2. ordinary partitions p(j) */
    new_p[0] = 1;                /* p(0)=1 */
    for (idx_t part = 1; part <= N; ++part) {
        for (idx_t sum = part; sum <= N; ++sum) {
            new_p[sum] += new_p[sum - part];
        }
    }

    /* 3. distinct-part partitions d(m) */
    new_d[0] = 1;                /* d(0)=1 */
    for (idx_t part = 1; part <= N; ++part) {
        for (idx_t sum = N; sum >= part; --sum) {
            new_d[sum] += new_d[sum - part];
        }
    }

    /* 4. overpartitions op(m) = convolution of p and d */
    for (idx_t j = 0; j <= N; ++j) {
        seq_t pj = new_p[j];
        if (!pj) continue;
        for (idx_t k = 0; k <= N - j; ++k) {
            new_op[j + k] += pj * new_d[k];
        }
    }

    /* 5. a(n) = convolution of sigma and op */
    for (idx_t k = 1; k <= N; ++k) {
        seq_t sval = new_sigma[k];
        if (!sval) continue;
        for (idx_t m = 0; m <= N - k; ++m) {
            new_a[k + m] += sval * new_op[m];
        }
    }

    /* free old arrays if any */
    free(sigma_arr);
    free(p_arr);
    free(d_arr);
    free(op_arr);
    free(a_arr);

    /* assign new arrays to static state */
    sigma_arr = new_sigma;
    p_arr     = new_p;
    d_arr     = new_d;
    op_arr    = new_op;
    a_arr     = new_a;
    cur_max   = N;
}

/* ---------- exported function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1, but define a(0)=0 */

    if (n > cur_max) {
        /* recompute values up to n */
        recompute_upto(n);
    }

    return a_arr[n];
}
