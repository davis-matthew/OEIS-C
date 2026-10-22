/*
 * A321355  Expansion of Product_{k>=0} (1 + x^(4^k))^(4^(k+1)).
 * https://oeis.org/A321355
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k≥0} (1 + x^{4^k})^{4^{k+1}}
 * Range:     u8 n <= 8, u16 n <= 25, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation builds the generating polynomial iteratively. */

#include <stdlib.h>
#include <stdint.h>

/* Helpers to allocate/free arrays of seq_t, works for both C and C++ */
#if defined(__cplusplus)
static seq_t *alloc_seq_array(idx_t sz) {
    return new seq_t[sz]();     /* default-initialised elements */
}
static void free_seq_array(seq_t *arr, idx_t /*sz*/) {
    delete[] arr;
}
#else
static seq_t *alloc_seq_array(idx_t sz) {
    return (seq_t*)calloc((size_t)sz, sizeof(seq_t));
}
static void free_seq_array(seq_t *arr, idx_t /*sz*/) {
    free(arr);
}
#endif

/* Compute the nth term of sequence A321355. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    idx_t size = n + 1;
    seq_t *cur = alloc_seq_array(size);
    cur[0] = (seq_t)1;                     /* start polynomial 1 */

    idx_t step = 1;                         /* 4^k */
    while (step <= n) {
        seq_t total = (seq_t)(step * 4);        /* 4^{k+1} */

        /* maximum exponent choice for this factor */
        idx_t max_j = step * 4ULL;             /* 4^{k+1} */
        idx_t max_possible = n / step;         /* j*4^k ≤ n */
        if (max_j > max_possible) max_j = max_possible;

        /* binomial coefficients C(4^{k+1}, j) for j=0..max_j */
        seq_t *binom = alloc_seq_array(max_j + 1);
        binom[0] = (seq_t)1;
        for (idx_t j = 1; j <= max_j; ++j) {
            seq_t num = binom[j-1] * (total - (seq_t)(j-1));
            binom[j] = num / (seq_t)j;          /* division exact */
        }

        /* convolution: multiply current polynomial by this factor */
        seq_t *next = alloc_seq_array(size);
        for (idx_t s = 0; s <= n; ++s) {
            if (cur[s] == (seq_t)0) continue;
            for (idx_t j = 0; j <= max_j; ++j) {
                idx_t off = j * step;
                if (s + off > n) break;
                next[s + off] += cur[s] * binom[j];
            }
        }

        /* free old data */
        free_seq_array(cur, size);
        free_seq_array(binom, max_j + 1);

        cur = next;
        step <<= 2;                           /* next power of 4 */
    }

    seq_t result = cur[n];
    free_seq_array(cur, size);
    return result;
}
