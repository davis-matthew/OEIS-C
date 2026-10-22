/*
 * A372211  a(n) = [x^n] f(x)^n, where f(x) = (1 - x^5)^5/((1 - x^2)^2 * (1 -
 *          x^3)^3).
 * https://oeis.org/A372211
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [x^n] ((1 - x^5)^5 / ((1 - x^2)^2 (1 - x^3)^3))^n
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

/* Compute one term a(n) by the combinatorial formula */
static seq_t compute_a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* bounds for the binomial tables */
    idx_t i_max = n / 5;
    idx_t j_max = n / 2;
    idx_t k_max = n / 3;

    /* Allocate tables */
    seq_t *bin_i = seq_alloc_new_array(i_max + 1);
    seq_t *bin_j = seq_alloc_new_array(j_max + 1);
    seq_t *bin_k = seq_alloc_new_array(k_max + 1);

    /* bin_i[i] = C(5*n, i) */
    bin_i[0] = (seq_t)1;
    for (idx_t i = 1; i <= i_max; ++i) {
        seq_t num = (seq_t)(5ULL * n - i + 1);
        bin_i[i] = bin_i[i - 1] * num / (seq_t)i;
    }

    /* bin_j[j] = C(2*n + j - 1, j) */
    bin_j[0] = (seq_t)1;
    for (idx_t j = 1; j <= j_max; ++j) {
        seq_t num = (seq_t)(2ULL * n + j - 1);
        bin_j[j] = bin_j[j - 1] * num / (seq_t)j;
    }

    /* bin_k[k] = C(3*n + k - 1, k) */
    bin_k[0] = (seq_t)1;
    for (idx_t k = 1; k <= k_max; ++k) {
        seq_t num = (seq_t)(3ULL * n + k - 1);
        bin_k[k] = bin_k[k - 1] * num / (seq_t)k;
    }

    seq_t pos_sum = (seq_t)0;
    seq_t neg_sum = (seq_t)0;

    /* Loop over i and k, compute j as needed */
    for (idx_t i = 0; i <= i_max; ++i) {
        idx_t m = n - 5 * i;
        int sign = (i & 1) ? -1 : 1;           /* (−1)^i */
        seq_t bi = bin_i[i];

        idx_t k_lim = m / 3;
        for (idx_t k = 0; k <= k_lim; ++k) {
            idx_t rem = m - 3 * k;
            if (rem & 1U)        /* need even remainder for j integer */
                continue;
            idx_t j = rem >> 1;   /* j = (m − 3k) / 2 */
            seq_t term = bi * bin_j[j] * bin_k[k];
            if (sign < 0)
                neg_sum += term;
            else
                pos_sum += term;
        }
    }

    seq_free_array(bin_i);
    seq_free_array(bin_j);
    seq_free_array(bin_k);

    return pos_sum - neg_sum;
}

/* Public entry point */
seq_t a(idx_t n)
{
    return compute_a(n);
}
