/*
 * A373234  Relative of Hofstadter Q-sequence: a(n) = max(0, n+196) for n <=
 *          0; a(n) = a(n-a(n-1)) + a(n-a(n-2)) + a(n-a(n-3)) for n > 0.
 * https://oeis.org/A373234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = max(0, n+196) for n <= 0; a(n) = a(n-a(n-1)) +
 *            a(n-a(n-2)) + a(n-a(n-3)) for n > 0; Offset: 1 (a(1) is the
 *            first term)
 * Range:     u8 n <= 23, u16 n <= 223, u32 n <= 223, u64 n <= 223,
 *            u128 n <= 223, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence has exactly 223 positive terms. a(223) = 0.  This
   implementation computes and caches values up to the largest n
   requested; values beyond 223 are not defined. */

#include <stdbool.h>

/* Cache of computed values.  cache[0] holds a(0)=196.  Valid indices: 0..223. */
static seq_t cache[224];
/* Indicates how many elements of cache have been computed (maximum index reached). */
static idx_t max_computed = 0;

/* Return a(k) for any signed integer k.  For k <= 0 we use the
   definition a(k) = max(0, k+196).  For positive k we assume the
   value has already been computed and stored in cache. */
static inline seq_t get_signed(long long k)
{
    if (k <= 0) {
        long long v = k + 196;
        return (v > 0) ? (seq_t)v : (seq_t)0;
    }
    /* k is guaranteed to be in [1, max_computed] when used. */
    return cache[(idx_t)k];
}

/* Main sequence function.  Computes all values up to the requested
   index n, caching results for future calls. */
seq_t a(idx_t n)
{
    /* Support the trivial case n==0 explicitly; not used by OEIS offset. */
    if (n == 0)
        return (seq_t)196;

    /* The sequence is defined only up to n = 223. */
    if (n > 223)
        return (seq_t)0;   /* undefined behaviour beyond 223 */

    /* Ensure cache[0] = a(0) = 196. */
    if (max_computed == 0) {
        cache[0] = (seq_t)196;
        max_computed = 0;
    }

    /* Compute values from max_computed+1 up to n. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {

        /* Compute a(i-1), a(i-2), a(i-3).  For indices < 0 use get_signed. */
        seq_t a_im1 = (i >= 1) ? cache[i-1] : get_signed((long long)i-1);
        seq_t a_im2 = (i >= 2) ? cache[i-2] : get_signed((long long)i-2);
        seq_t a_im3 = (i >= 3) ? cache[i-3] : get_signed((long long)i-3);

        /* Recurrence indices.  Use signed arithmetic to allow negative results. */
        long long k1 = (long long)i - (long long)a_im1;
        long long k2 = (long long)i - (long long)a_im2;
        long long k3 = (long long)i - (long long)a_im3;

        /* Compute the sum using get_signed to handle negative target indices. */
        cache[i] = get_signed(k1) + get_signed(k2) + get_signed(k3);
    }

    max_computed = n;
    return cache[n];
}
