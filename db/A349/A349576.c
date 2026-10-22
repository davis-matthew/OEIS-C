/*
 * A349576  Recurrence a(1) = 1, a(2) = 5; a(n) = (a(n-1) +
 *          a(n-2))/GCD(a(n-1),a(n-2)) + 1.
 * https://oeis.org/A349576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(2) = 5; a(n) = (a(n-1)+a(n-2))/gcd(a(n-1),a(n-2)) +
 *            1
 * Range:     u8 n <= 13, u16 n <= 36, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"

/* Return the n‑th term of sequence A349576.  Indexing starts at 1. */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)5;

    /* static state to memoise previously computed terms */
    static seq_t *cache = NULL;   /* array of computed terms (0‑based index) */
    static size_t cap   = 0;      /* current allocated size of cache      */
    static size_t len   = 0;      /* number of terms already filled       */

    /* Ensure the cache is large enough for the requested n */
    if ((size_t)n > cap) {
        size_t new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {         /* allocation failure: abort the computation */
            /* In the unlikely event of failure we simply return 0. */
            return (seq_t)0;
        }
        cache = tmp;
        cap = new_cap;
    }

    /* initialise the first two terms if this is the first call */
    if (len < 2) {
        cache[0] = (seq_t)1;   /* a(1) */
        cache[1] = (seq_t)5;   /* a(2) */
        len = 2;
    }

    /* compute any missing terms up to n */
    for (size_t i = len; i < (size_t)n; ++i) {
        seq_t prev1 = cache[i - 1]; /* a(i)   */
        seq_t prev2 = cache[i - 2]; /* a(i-1) */
        seq_t g = gcd(prev1, prev2);
        seq_t sum = prev1 + prev2;
        cache[i] = sum / g + (seq_t)1;
    }
    len = (size_t)n;

    return cache[n - 1];
}
