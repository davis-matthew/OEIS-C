/*
 * A361840  Square array T(n,k), n>=0, k>=0, read by antidiagonals downwards,
 *          where column k is the expansion of 1/(1 - 9*x*(1 - x)^k)^(1/3).
 * https://oeis.org/A361840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = T(i,j) where i+j = ⌊(√(8*n+1)-1)/2⌋, i = n -
 *            ⌊(i+j)*(i+j+1)/2⌋, j = (i+j) - i. The square array T(n,k) is
 *            defined by T(0,k) = 1, n*T(n,k) = 3 * Σ_{j=0..k} (-1)^j *
 *            binomial(k,j) * (3*n-2-2*j) * T(n-1-j,k) (with T(m,k)=0 for m<0)
 * Range:     i8 n <= 8, i16 n <= 19, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>

/* helper: binomial for small non‑negative integers using seq_t */
static seq_t binom_small(idx_t n, idx_t k) {
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;          /* use symmetry */
    seq_t res = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i); /* multiply first, then divide */
        res = res / (seq_t)i;
    }
    return res;
}

/* compute the anti‑diagonal index t for a given n */
static idx_t antidiag(idx_t n) {
    double d = sqrt((double)(8.0 * (long double)n + 1.0));
    idx_t t = (idx_t)((d - 1.0) / 2.0);
    /* correct possible rounding errors */
    while ((t + 1) * (t + 2) / 2 <= n) ++t;
    while (t * (t + 1) / 2 > n) --t;
    return t;
}

/* dynamically build T up to dimension 'dim' (dim = t_max+1) */
static void build_T(size_t dim, seq_t **storage) {
    /* allocate new storage */
    seq_t *buf = (seq_t *)calloc(dim * dim, sizeof(seq_t));
    if (!buf) return; /* allocation failure: nothing to do */

    /* base row n = 0 */
    for (size_t k = (size_t)0; k < dim; ++k)
        buf[0 * dim + k] = (seq_t)1;

    /* other rows */
    for (size_t n = (size_t)1; n < dim; ++n) {
        for (size_t k = (size_t)0; k < dim; ++k) {
            seq_t sum = (seq_t)0;
            /* j runs from 0 to k, but terms with j > n-1 vanish (n-1-j < 0) */
            for (size_t j = (size_t)0; j <= k; ++j) {
                if (j > n - 1) continue;
                seq_t bin = binom_small((idx_t)k, (idx_t)j);
                seq_t factor = (seq_t)3 * (seq_t)n - (seq_t)2 - (seq_t)2 * (seq_t)j;
                seq_t term = bin * factor * buf[(n - 1 - j) * dim + k];
                if (j & 1U) sum -= term; else sum += term;
            }
            seq_t res = (seq_t)3 * sum;
            res = res / (seq_t)n; /* integer division, result is exact */
            buf[n * dim + k] = res;
        }
    }
    *storage = buf;
}

/* static cached data */
static seq_t *T_storage = NULL;
static size_t T_dim = 0;

/* main sequence function */
seq_t a(idx_t n) {
    /* determine required dimension */
    idx_t t = antidiag(n);
    size_t required = (size_t)(t + 1); /* dimension needed */
    if (required > T_dim) {
        /* rebuild table for new dimension */
        free(T_storage);
        build_T(required, &T_storage);
        T_dim = required;
    }
    /* locate (i,j) in the antidiagonal */
    idx_t base = t * (t + 1) / 2;
    idx_t i = n - base;      /* row index */
    idx_t j = t - i;         /* column index */
    return T_storage[i * T_dim + j];
}
