/*
 * A245669  Expansion of q * f(q, q^5)^3 in powers of q where f() is
 *          Ramanujan's two-variable theta function.
 * https://oeis.org/A245669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of q^n in x*(Sum_{k in Z} q^(3*k^2+2*k))^3
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static idx_t current_max = 0;
static seq_t *cache = NULL;

/* compute a(n) for n >= 1 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* safety */

    /* Return cached value if already computed */
    if (n <= current_max) {
        return cache[(size_t)n];
    }

    /* new maximum needed */
    idx_t max_n   = n;
    size_t size_s = (size_t)(max_n);            /* indices 0 .. max_n-1 */
    size_t len_conv1 = 2 * size_s;              /* for sums of two indices */

    /* allocate helper arrays */
    seq_t *s = (seq_t *)calloc(size_s, sizeof(seq_t));
    if (!s) return (seq_t)0;

    /* indices where s[i] == 1 */
    idx_t *indices = NULL;
    size_t idx_cap = 0, idx_cnt = 0;
    auto_add_idx:
    ((void)(0)); /* placeholder for macro style */
    /* helper to grow indices array */
    #define ADD_IDX(val)                                 \
        do {                                            \
            if (idx_cnt >= idx_cap) {                    \
                idx_cap = (idx_cap ? idx_cap * 2 : 16);   \
                indices = (idx_t *)realloc(indices, idx_cap * sizeof(idx_t)); \
            }                                            \
            indices[idx_cnt++] = (idx_t)(val);            \
        } while (0)

    /* positive k (including 0) */
    for (idx_t k = 0;; ++k) {
        size_t val = (size_t)(3ULL * k * k + 2ULL * k);
        if (val >= size_s) break;
        s[(size_t)val] = (seq_t)1;
        ADD_IDX(val);
    }

    /* negative k (k = -1, -2, …) */
    for (idx_t kp = 1;; ++kp) {
        size_t val = (size_t)(3ULL * kp * kp - 2ULL * kp);
        if (val >= size_s) break;
        s[(size_t)val] = (seq_t)1;
        ADD_IDX(val);
    }

    /* convolution s * s -> conv1 */
    seq_t *conv1 = (seq_t *)calloc(len_conv1 + 1, sizeof(seq_t));
    if (!conv1) { free(s); free(indices); return (seq_t)0; }

    for (size_t ii = 0; ii < idx_cnt; ++ii) {
        idx_t i = indices[ii];
        for (size_t jj = 0; jj < idx_cnt; ++jj) {
            idx_t j   = indices[jj];
            idx_t sum = i + j;
            conv1[(size_t)sum] += (seq_t)1;
        }
    }

    /* convolution conv1 * s -> a_noShift (coeffs for exponents 0 .. size_s-1) */
    seq_t *a_noShift = (seq_t *)calloc(size_s, sizeof(seq_t));
    if (!a_noShift) { free(s); free(indices); free(conv1); return (seq_t)0; }

    for (size_t kk = 0; kk < idx_cnt; ++kk) {
        idx_t k = indices[kk];
        for (size_t t = (size_t)k; t < size_s; ++t) {
            a_noShift[t] += conv1[t - k];
        }
    }

    /* build cache up to max_n */
    seq_t *new_cache = (seq_t *)calloc((size_t)(max_n + 1), sizeof(seq_t));
    if (!new_cache) { free(s); free(indices); free(conv1); free(a_noShift); return (seq_t)0; }

    for (idx_t i = 1; i <= max_n; ++i) {
        new_cache[(size_t)i] = a_noShift[(size_t)(i - 1)];
    }

    /* replace old cache */
    if (cache) free(cache);
    cache = new_cache;
    current_max = max_n;

    /* free temporary arrays */
    free(s);
    free(indices);
    free(conv1);
    free(a_noShift);

    return cache[(size_t)n];
}
