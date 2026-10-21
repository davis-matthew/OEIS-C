/*
 * A267827  Number of closed indecomposable linear lambda terms with 2n+1
 *          applications and abstractions.
 * https://oeis.org/A267827
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=2; a(n) = (6*n-2)*a(n-1) + Sum_{k=1..n-2}
 *            (6*k+2)*a(k)*a(n-1-k), for n>=2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;       /* stores a(n) values */
static seq_t *b_cache = NULL;       /* stores (6*i+2)*a(i) values */
static idx_t cache_size = 0;        /* number of values stored (max index +1) */
static size_t cache_capacity = 0;   /* allocated array size */

/* Ensure that a_cache and b_cache contain values up to index n. */
static void ensure_cache(idx_t n)
{
    if (n < cache_size) return;    /* already computed */
    /* grow capacity if necessary */
    if (n + 1 > cache_capacity) {
        size_t new_cap = cache_capacity ? cache_capacity * 2 : 16;
        while (new_cap <= n + 1) new_cap <<= 1;
        seq_t *new_a = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        seq_t *new_b = (seq_t *)realloc(b_cache, new_cap * sizeof(seq_t));
        a_cache = new_a;
        b_cache = new_b;
        cache_capacity = new_cap;
    }
    /* compute missing terms */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0) {
            a_cache[0] = (seq_t)1;
            b_cache[0] = ((seq_t)6 * 0 + 2) * a_cache[0];
        } else if (i == 1) {
            a_cache[1] = (seq_t)2;
            b_cache[1] = ((seq_t)6 * 1 + 2) * a_cache[1];
        } else {
            /* sum_{k=1}^{i-2} (6*k+2)*a(k)*a(i-1-k) */
            seq_t sum = 0;
            for (idx_t k = 1; k <= i-2; ++k) {
                seq_t term = ((seq_t)6 * k + 2);
                term *= a_cache[k];
                term *= a_cache[i-1-k];
                sum += term;
            }
            seq_t coeff = ((seq_t)6 * i - 2);
            a_cache[i] = coeff * a_cache[i-1] + sum;
            b_cache[i] = ((seq_t)6 * i + 2) * a_cache[i];
        }
    }
    cache_size = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return a_cache[n];
}
