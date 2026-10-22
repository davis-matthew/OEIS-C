/*
 * A390656  G.f. A(x) satisfies A(x) = 1 / ((1 - x) * (1 - 2 * x * (1 + x) *
 *          A(x^2))).
 * https://oeis.org/A390656
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + 2 * Sum_{k=0..n-1} a(floor(k/2)) * a(n-1-k)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Maximum n we expect to support.   */
#define N_MAX 10000

/* Static memoization array: a[0] … a[N_MAX]. */
static seq_t cache[N_MAX + 1];
/* Number of entries that have already been computed. */
static idx_t cache_len = 0;

/* The actual sequence function. */
seq_t a(idx_t n)
{
    /* Initialise cache[0] on first call. */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* If n exceeds our generous bound, treat it as if we had reached it
       via previous calls: the function is not required to handle larger n. */
    if (n > N_MAX) {
        /* Cannot compute; in the ordinary build this will never happen. */
        return (seq_t)0;
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k < i; ++k) {
            /* floor(k/2) works for unsigned indices. */
            seq_t left  = cache[k >> 1];
            seq_t right = cache[i - 1 - k];
            sum += left * right;
        }
        cache[i] = (seq_t)1 + (sum << 1);   /* 1 + 2*sum */
    }
    cache_len = n + 1;

    return cache[n];
}
