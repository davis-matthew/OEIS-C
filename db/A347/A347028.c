/*
 * A347028  a(1) = 1; a(n+1) = -Sum_{k=1..n} a(floor(n/k)).
 * https://oeis.org/A347028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1) = 1; a(n+1) = -∑_{k=1}^{n} a(floor(n/k)) (OEIS A347028)
 * Range:     i8 n <= 48, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Return the n-th term of the sequence.  idx_t is an unsigned 64‑bit
   type, seq_t is a signed integer type or an arbitrary‑precision class. */
seq_t a(idx_t n)
{
    /* static memoisation buffer: indices 1..cache_size-1 are valid */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;      /* allocated size, includes index 0 */
    static idx_t computed = 0;        /* highest index that has been filled */

    if (n < 1) return 0;              /* not part of the sequence */

    /* ensure buffer is large enough for indices up to n */
    if (cache_size < n + 1) {
        idx_t new_size = (cache_size == 0 ? 16 : cache_size * 2);
        if (new_size < n + 1) new_size = n + 1;

        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failure – shouldn't happen */
        cache = tmp;
        cache_size = new_size;
    }

    /* initialise the base value if needed */
    if (computed < 1) {
        cache[1] = (seq_t)1;
        computed = 1;
    }

    /* compute missing values up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        idx_t m = i - 1;                      /* the n in the recurrence */
        seq_t sum = 0;

        idx_t k = 1;
        while (k <= m) {
            idx_t d = m / k;                   /* floor(m/k) */
            idx_t next_k = m / d + 1;          /* first k with a smaller quotient */
            seq_t cnt = (seq_t)(next_k - k);   /* how many times d occurs */

            sum += cache[d] * cnt;
            k = next_k;
        }
        cache[i] = -sum;
    }
    computed = n;

    return cache[n];
}
