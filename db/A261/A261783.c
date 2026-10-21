/*
 * A261783  Number of compositions of n where each part i is marked with a
 *          word of length i over an n-ary alphabet whose letters appear in
 *          alphabetical order.
 * https://oeis.org/A261783
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; a(n)=∑_{i=1}^{n} C(n+i−1,i) · a(n−i)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* static cache for previously computed values */
static seq_t *cache_a = NULL;
static size_t cache_a_size = 0;
static seq_t *cache_b = NULL;   /* binomial coefficients C(n+i−1,i) */
static size_t cache_b_size = 0;

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;           /* a(0) is defined to be 1 */

    size_t m = (size_t)n;
    size_t needed = m + 1;  /* need indices 0..n */

    /* Allocate or resize cache_a */
#if defined(__cplusplus)
    if (needed > cache_a_size) {
        delete[] cache_a;
        cache_a = new seq_t[needed];
        cache_a_size = needed;
    }
    if (needed > cache_b_size) {
        delete[] cache_b;
        cache_b = new seq_t[needed];
        cache_b_size = needed;
    }
#else
    if (needed > cache_a_size) {
        free(cache_a);
        cache_a = (seq_t*)malloc(needed * sizeof(seq_t));
        cache_a_size = needed;
    }
    if (needed > cache_b_size) {
        free(cache_b);
        cache_b = (seq_t*)malloc(needed * sizeof(seq_t));
        cache_b_size = needed;
    }
#endif

    seq_t *a = cache_a;
    seq_t *b = cache_b;

    a[0] = 1;          /* base case */

    /* b[i] = C(n + i - 1, i) for i = 1..n */
    b[0] = 1;          /* unused, but harmless */
    b[1] = (seq_t)n;   /* C(n,1) = n */

    for (size_t i = 2; i <= m; ++i) {
        seq_t numer = b[i - 1] * ((seq_t)n + (seq_t)i - 1);
        b[i] = numer / (seq_t)i;           /* exact division */
    }

    /* DP: a[m] = ∑_{i=1}^{m} b[i] * a[m-i]   */
    for (size_t j = 1; j <= m; ++j) {
        seq_t sum = 0;
        for (size_t i = 1; i <= j; ++i)
            sum += b[i] * a[j - i];
        a[j] = sum;
    }

    return a[m];
}
