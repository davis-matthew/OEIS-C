/*
 * A368446  Expansion of e.g.f. exp(-x) / (1 + log(1 - 2*x)).
 * https://oeis.org/A368446
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (-1)^n + Sum_{k=1..n} 2^k * (k-1)! * binomial(n,k) *
 *            a(n-k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/seq_realloc.h"

/* static cache for computed values of a(n) */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;   /* next index that needs to be computed */

/* cache for coefficients c_k = 2^k * (k-1)! */
static seq_t *coeff = NULL;
static size_t coeff_cap = 0;

/* enlarge an array to hold at least need+1 elements (indices up to need) */
static void enlarge_array(seq_t **arr, size_t *cap, size_t need)
{
    if (need < *cap) return;
    size_t new_cap = (*cap == 0 ? 1 : *cap);
    while (new_cap <= need) new_cap <<= 1;
    *arr = seq_realloc(*arr, *cap, new_cap);
    *cap = new_cap;
}

static void compute_up_to(idx_t n)
{
    /* ensure storage for cache and coeff up to index n */
    enlarge_array(&cache,  &cache_cap,  n);
    enlarge_array(&coeff, &coeff_cap,  n);

    /* fill coefficient array up to n */
    for (idx_t k = 1; k <= n; ++k) {
        if (k == 1) {
            coeff[1] = (seq_t)2; /* 2^1 * 0! = 2 */
        } else {
            /* c_k = c_{k-1} * 2 * (k-1) */
            seq_t tmp = coeff[k - 1];
            tmp = tmp * (seq_t)2 * (seq_t)(k - 1);
            coeff[k] = tmp;
        }
    }

    /* compute a(i) for all i not yet computed, up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
            continue;
        }

        seq_t sum = (seq_t)0;
        seq_t binom = (seq_t)i; /* binomial(i,1) */

        for (idx_t k = 1; k <= i; ++k) {
            /* term = binom * coeff[k] * a(i-k) */
            seq_t term = binom;
            term = term * coeff[k];
            term = term * cache[i - k];
            sum += term;

            if (k < i) {
                /* binom(i,k+1) = binom(i,k) * (i-k) / (k+1) */
                seq_t numer = binom * (seq_t)(i - k);
                binom = numer / (k + 1);
            }
        }

        /* add (-1)^i */
        if ((i & 1ULL) == 0) {
            sum += (seq_t)1;
        } else {
            sum -= (seq_t)1;
        }

        cache[i] = sum;
    }

    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    compute_up_to(n);
    return cache[n];
}
