/*
 * A251625  Numbers n such that the octagonal number N(n) is the sum of three
 *          consecutive octagonal numbers.
 * https://oeis.org/A251625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2703*a(n‑1) - 2703*a(n‑2) + a(n‑3) Initial values (n =
 *            1,2,3): a(1) = 483 a(2) = 1304163 a(3) = 3523847043
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 5, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function follows the recurrence and memoises computed terms.
   It works for any seq_t type supported by the build (fixed or
   arbitrary‑precision) and returns the correct value for all
   n >= 1.  The cache is grown by doubling to keep amortised O(1)
   per additional term. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* The sequence has offset 1; values for n < 1 are unused. */
    if (n == 0) return 0;

    static seq_t *cache  = NULL;   /* cache[0] unused, cache[1]..cache[max] filled */
    static idx_t cache_size = 0;   /* size allocated (excluding the unused 0 slot) */
    static idx_t last_computed = 0;/* biggest n for which cache[n] is valid */

    /* Allocate initial cache space if this is the first call. */
    if (cache == NULL) {
        cache_size = 8;                           /* arbitrary small start size */
        cache = (seq_t*)malloc((cache_size + 1) * sizeof(seq_t));
        if (!cache) exit(1);                     /* allocation failure → abort */
        cache[0] = 0;                            /* unused element */
    }

    /* Grow cache if needed to hold index n. */
    if (n > cache_size) {
        idx_t new_size = cache_size * 2;
        if (new_size < n) new_size = n;
        seq_t *tmp = (seq_t*)realloc(cache, (new_size + 1) * sizeof(seq_t));
        if (!tmp) { free(cache); exit(1); }
        cache = tmp;
        cache_size = new_size;
    }

    /* Ensure that we have initial values up to n or at least up to 3. */
    if (last_computed < 3) {
        cache[1] = 483;
        cache[2] = 1304163;
        cache[3] = 3523847043;
        last_computed = 3;
    }

    /* Compute missing terms up to n using the recurrence. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t t1 = cache[i - 1];
        t1 *= (seq_t)2703;
        seq_t t2 = cache[i - 2];
        t2 *= (seq_t)2703;
        t1 = t1 - t2;
        t1 = t1 + cache[i - 3];
        cache[i] = t1;
    }
    last_computed = n;

    return cache[n];
}
