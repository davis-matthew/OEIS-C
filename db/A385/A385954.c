/*
 * A385954  a(0) = 1; a(n) = Sum_{k=0..n-1} binomial(k+5,5) * a(k) * a(n-1-k).
 * https://oeis.org/A385954
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A385954
   a(0) = 1
   a(n) = Σ_{k=0}^{n-1} binomial(k+5,5) * a(k) * a(n-1-k)   (n ≥ 1)
*/
#include <stddef.h>
#include <stdint.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

/* internal cache for a(n) */
static seq_t *cache = NULL;         /* array of a-values */
static size_t cache_cap = 0;        /* allocated capacity */

/* internal cache for binomial(k+5,5) , k ≥ 0 */
static seq_t *binom_cache = NULL;   /* array of binomials */
static idx_t binom_computed = 0;    /* number of binome entries already computed */
static size_t binom_cap = 0;        /* allocated capacity */

/* ensure cache has capacity for indices 0..n */
static void ensure_cache_capacity(idx_t n) {
    if (cache_cap > (size_t)n) return;
    size_t new_cap = (cache_cap == 0) ? 1 : cache_cap;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    seq_t *new_arr = seq_alloc_new_array(new_cap);
    for (size_t i = 0; i < cache_cap; ++i) new_arr[i] = cache[i];
    for (size_t i = cache_cap; i < new_cap; ++i) new_arr[i] = (seq_t)0;
    seq_free_array(cache);
    cache = new_arr;
    cache_cap = new_cap;
}

/* ensure binom_cache holds binomial(k+5,5) for 0 ≤ k < n */
static void ensure_binom_capacity(idx_t n) {
    if (n <= binom_computed) return;

    /* enlarge storage if necessary */
    if (binom_cap <= (size_t)n) {
        size_t new_cap = (binom_cap == 0) ? 1 : binom_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *new_arr = seq_alloc_new_array(new_cap);
        for (size_t i = 0; i < binom_computed; ++i) new_arr[i] = binom_cache[i];
        for (size_t i = binom_computed; i < new_cap; ++i) new_arr[i] = (seq_t)0;
        seq_free_array(binom_cache);
        binom_cache = new_arr;
        binom_cap = new_cap;
    }

    /* compute missing binomial values */
    if (binom_computed == 0) {
        binom_cache[0] = (seq_t)1;   /* C(5,5) */
        binom_computed = 1;
    }
    for (idx_t k = binom_computed; k < n; ++k) {
        /* C(k+5,5) = C(k+4,5) * (k+5) / k   for k ≥ 1 */
        binom_cache[k] = binom_cache[k - 1] * (seq_t)(k + 5) / (seq_t)k;
    }
    binom_computed = n;
}

/* public function */
seq_t a(idx_t n) {
    if (n == 0) {
        ensure_cache_capacity(0);
        cache[0] = (seq_t)1;
        return (seq_t)1;
    }

    ensure_cache_capacity(n);
    ensure_binom_capacity(n);   /* need binomials up to index n‑1 */

    for (idx_t i = 1; i <= n; ++i) {
        if (cache[i] != (seq_t)0) continue;   /* already computed */

        seq_t sum = (seq_t)0;
        idx_t last = i - 1;
        idx_t half = last >> 1;              /* floor(last/2) */

        for (idx_t k = 0; k <= half; ++k) {
            seq_t prod = cache[k] * cache[last - k];
            seq_t term1 = prod * binom_cache[k];
            sum += term1;
            if (k != last - k) {
                seq_t term2 = prod * binom_cache[last - k];
                sum += term2;
            }
        }
        cache[i] = sum;
    }
    return cache[n];
}
