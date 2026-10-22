/*
 * A321357  Expansion of Product_{k>=0} (1 + x^(5^k))^(5^(k+1)).
 * https://oeis.org/A321357
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k≥0} (1 + x^(5^k))^(5^(k+1))
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/new_array.h"

static void delete_array(seq_t *arr) {
    if (!arr) return;
#ifdef __cplusplus
    delete[] arr;
#else
    free(arr);
#endif
}

/* cached values */
static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* compute all terms up to nmax (inclusive) */
static void compute_up_to(idx_t nmax) {
    /* start with the trivial product 1 */
    seq_t *prod = new_array(nmax + 1);
    prod[0] = (seq_t)1;

    idx_t pow5k = 1;      /* 5^k   */
    idx_t N = 5;          /* 5^(k+1) */

    while (pow5k <= nmax) {
        /* maximum j so that j*5^k <= nmax and j <= N */
        idx_t jmax = nmax / pow5k;
        if (jmax > N) jmax = N;

        /* binomial coefficients C(N, j) for j = 0..jmax */
        seq_t *binom = new_array(jmax + 1);
        binom[0] = (seq_t)1;
        for (idx_t j = 1; j <= jmax; ++j) {
            binom[j] = (binom[j - 1] * (seq_t)(N - j + 1)) / (seq_t)j;
        }

        /* convolution prod * (1 + x^(5^k))^N */
        seq_t *tmp = new_array(nmax + 1);
        for (idx_t j = 0; j <= jmax; ++j) {
            idx_t shift = j * pow5k;
            seq_t coeff = binom[j];
            for (idx_t i = shift; i <= nmax; ++i) {
                tmp[i] += prod[i - shift] * coeff;
            }
        }

        delete_array(binom);
        delete_array(prod);
        prod = tmp;

        /* advance to next power of 5 */
        pow5k *= (idx_t)5;
        N *= (idx_t)5;
    }

    /* replace old cache */
    delete_array(cache);
    cache = prod;
    cache_size = nmax + 1;
}

/* required public function */
seq_t a(idx_t n) {
    if (!cache || n >= cache_size)          /* enlarge cache if needed */
        compute_up_to(n);
    return cache[n];
}
