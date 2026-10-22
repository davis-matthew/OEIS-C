/*
 * A327542  A linear divisibility sequence of order 8.
 * https://oeis.org/A327542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 7*a(n-2) - 6*a(n-3) + 4*a(n-4) + 6*a(n-5) +
 *            7*a(n-6) - 2*a(n-7) - a(n-8) for n > 8 Initial values: a(1)=1,
 *            a(2)=2, a(3)=16, a(4)=36, a(5)=171, a(6)=512, a(7)=2087,
 *            a(8)=6984
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

/* The following uses malloc/realloc, which are allowed in both C and C++.
   Explicit casts are used to avoid C++ conversion errors. */

static seq_t *cache = NULL;   /* cache[i] contains a(i+1) for i >= 0   */
static size_t cache_len = 0;  /* number of entries currently stored  */

static void grow_cache(size_t n)
{
    if (cache_len >= n)
        return;

    size_t old_len = cache_len;
    cache = (seq_t*)realloc(cache, sizeof(seq_t) * n);
    if (!cache)
        abort();

    /* Initial values (indices 1..8) */
    static const seq_t init_vals[8] = {1, 2, 16, 36, 171, 512, 2087, 6984};

    if (n <= 8) {
        for (size_t i = 0; i < n; ++i)
            cache[i] = init_vals[i];
        cache_len = n;
        return;
    }

    /* Guarantee that the first 8 terms are present before we compute higher ones */
    if (old_len == 0) {
        for (size_t i = 0; i < 8; ++i)
            cache[i] = init_vals[i];
        old_len = 8;
    }

    /* Compute terms from old_len+1 up to n (1‑based indices) */
    for (idx_t i = old_len + 1; i <= n; ++i) {
        seq_t a1 = cache[i - 2];
        seq_t a2 = cache[i - 3];
        seq_t a3 = cache[i - 4];
        seq_t a4 = cache[i - 5];
        seq_t a5 = cache[i - 6];
        seq_t a6 = cache[i - 7];
        seq_t a7 = cache[i - 8];
        seq_t a8 = cache[i - 9];

        /* Compute positive and negative contributions separately to avoid
           intermediate underflow.  The recurrence guarantees that positive
           contributions dominate, so the subtraction will not underflow. */
        seq_t pos = (seq_t)2 * a1 + (seq_t)7 * a2;
        pos += (seq_t)4 * a4 + (seq_t)6 * a5 + (seq_t)7 * a6;
        seq_t neg = (seq_t)6 * a3 + (seq_t)2 * a7 + a8;

        cache[i - 1] = pos - neg;
    }

    cache_len = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* Undefined for n<1, but return 0 safely */

    grow_cache((size_t)n);
    return cache[n - 1];
}
