/*
 * A349365  G.f. A(x) satisfies A(x) = 1 / (1 - x - x * A(x^2)).
 * https://oeis.org/A349365
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + Σ_{k=0..⌊(n-1)/2⌋} a(k) * a(n-1-2k); a(0)=1,
 *            a(1)=2
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 49, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/*   a : OEIS A349365
 *   idx_t : unsigned integer type for index (uint64_t)
 *   seq_t : unsigned integer type for term (up to 128b, big-integer, etc.)
 */
seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache   = NULL;
    static size_t   cap    = 0;
    static idx_t   max_n   = (idx_t)-1;   /* no terms computed yet */

    /* allocate initial cache with a(0)=1 */
    if (cache == NULL)
    {
        cap = 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;          /* allocation failure – unlikely */
        cache[0] = (seq_t)1;           /* a(0) */
        max_n = 0;
    }

    /* extend cache if needed */
    if (n >= cap)
    {
        size_t new_cap = cap;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;            /* double until big enough */

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;          /* allocation failure */
        cache = tmp;
        cap = new_cap;
    }

    /* compute terms up to n */
    for (idx_t i = max_n + 1; i <= n; ++i)
    {
        seq_t sum = 0;

        /* a(i) = a(i-1) + Σ_{k=0..⌊(i-1)/2⌋} a(k) * a(i-1-2k)   */
        if (i == 0)
        {
            sum = (seq_t)1;  /* already cached, but kept for clarity */
        }
        else
        {
            sum = cache[i - 1];          /* contribution a(i-1) */

            idx_t limit = (i - 1) / 2;   /* floor((i-1)/2) */
            for (idx_t k = 0; k <= limit; ++k)
            {
                /* index of second factor: i-1-2*k  (always <= i-1) */
                size_t j = (size_t)(i - 1 - 2 * k);
                sum += cache[k] * cache[j];
            }
        }

        cache[i] = sum;
    }

    max_n = n;
    return cache[n];
}
