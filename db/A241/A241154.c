/*
 * A241154  a(n)=1 for n <= s+k; thereafter a(n) =
 *          Sum(a(n-i-s-a(n-i-1)),i=0..k-1) where s=0, k=5.
 * https://oeis.org/A241154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hear, look
 * Formula:   not stated
 * Range:     u8 n <= 296, u16 n <= 9240, u32 n <= 9240, u64 n <= 9240,
 *            u128 n <= 9240, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for OEIS A241154 (s = 0, k = 5):
   a(n) = 1 , 1 ≤ n ≤ 5
   a(n) = Σ_{i=0}^{4} a( n - i - a(n-i-1) ) , n > 5
   The sequence is memoised so that calls can be made in any order. */
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* 1‑based cache of computed values; cache[0] unused. */
    static seq_t *cache = NULL;
    static size_t last = 0;          /* largest index computed so far     */

    if (n < 1)
        return 0;                    /* out of bounds – not used in tests */

    /* Ensure the cache can hold indices up to n (inclusive). */
    if ((size_t)n > last)
    {
        size_t new_size = (size_t)n + 1;          /* indices 0 .. n  */
        seq_t *tmp = (seq_t *)realloc(cache,
                                      new_size * sizeof(seq_t));
        if (!tmp)
            return 0;                       /* allocation failure */
        cache = tmp;

        /* Compute new terms from last+1 up to n. */
        for (size_t i = last + 1; i <= (size_t)n; ++i)
        {
            if (i <= 5)                     /* initial values */
            {
                cache[i] = 1;
            }
            else
            {
                seq_t sum = 0;
                for (size_t j = 0; j < 5; ++j)
                {
                    /* safe conversion: cache[i-j-1] fits into idx_t */
                    idx_t sub = (idx_t)cache[i - j - 1];
                    idx_t idx  = i - j - sub;   /* guaranteed ≥ 1 */
                    sum += cache[idx];
                }
                cache[i] = sum;
            }
        }
        last = (size_t)n;
    }
    return cache[n];
}
