/*
 * A349012  G.f. A(x) satisfies: A(x) = (1 + x * A(2*x)) / (1 - x * A(x)).
 * https://oeis.org/A349012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = 2^(n-1) * a(n-1) + Σ_{k=0}^{n-1} a(k) *
 *            a(n-k-1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow2.h"
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

/* Memoisation cache for sequence values */
static seq_t *seq_cache = NULL;
static size_t cache_len = 0;

/* Ensure cache can hold at least 'need' elements */
static void ensure_capacity(size_t need)
{
    if (need <= cache_len)
        return;
    size_t new_len = cache_len ? cache_len * 2 : 64;
    if (new_len < need)
        new_len = need;
    seq_t *new_arr = alloc_seq_array(new_len);
    if (!new_arr) {
        abort();
    }
    if (seq_cache) {
        for (size_t i = 0; i < cache_len; ++i)
            new_arr[i] = seq_cache[i];
        free_seq_array(seq_cache);
    }
    seq_cache = new_arr;
    cache_len = new_len;
}

/* Compute the n-th term of sequence A349012 */
seq_t a(idx_t n)
{
    ensure_capacity((size_t)n + 1);

    /* Base case: a(0) = 1 */
    if (n == 0) {
        if (seq_cache[0] == 0)
            seq_cache[0] = 1;
        return seq_cache[0];
    }

    /* Return cached value if already computed */
    if (seq_cache[n] != 0)
        return seq_cache[n];

    /* 2^(n-1) * a(n-1) */
    seq_t term = seq_cache[n - 1] * pow2((idx_t)(n - 1));

    /* Convolution sum Σ_{k=0}^{n-1} a(k) * a(n-k-1) */
    seq_t sum = 0;
    for (idx_t k = 0; k < n; ++k)
        sum += seq_cache[k] * seq_cache[n - 1 - k];

    seq_t res = term + sum;
    seq_cache[n] = res;
    return res;
}
