/*
 * A368767  a(n) = n! * (1 + Sum_{k=0..n} (-1)^k * binomial(k+2,3) / k!).
 * https://oeis.org/A368767
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*a(n-1) + (-1)^n * binomial(n+2,3), a(0) = 1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Memoized recurrence implementation */
seq_t a(idx_t n)
{
    /* Static cache to store computed values */
    static seq_t *cache = NULL;
    static idx_t max_computed = 0;
    static size_t capacity = 0;

    /* Initialise on first call */
    if (cache == NULL) {
        capacity = 1;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* out of memory – unlikely in tests */
        cache[0] = (seq_t)1;                 /* a(0) */
        max_computed = 0;
    }

    /* Extend cache if necessary */
    if (n > max_computed) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap > capacity) {
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;       /* out of memory – unlikely in tests */
            cache = tmp;
            capacity = new_cap;
        }

        /* Compute missing terms using the recurrence */
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            /* product part: i * a(i-1) */
            seq_t prod = (seq_t)i * cache[i - 1];

            /* Compute C(i+2,3) = i*(i+1)*(i+2)/6 with early division to avoid overflow */
            seq_t a = (seq_t)i;
            seq_t b = (seq_t)(i + 1);
            seq_t c = (seq_t)(i + 2);

            /* Divide by 2 */
            if ((a & 1) == 0) a /= 2;
            else if ((b & 1) == 0) b /= 2;
            else c /= 2;

            /* Divide by 3 */
            if (a % 3 == 0) a /= 3;
            else if (b % 3 == 0) b /= 3;
            else c /= 3;

            seq_t binom = a * b * c;         /* C(i+2,3) */

            /* Add or subtract based on parity of i */
            if ((i & 1) == 0)
                cache[i] = prod + binom;
            else
                cache[i] = prod - binom;
        }

        max_computed = n;
    }

    return cache[n];
}
