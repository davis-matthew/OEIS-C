/*
 * A380855  The unique sequence starting with a(0) = 1, a(1) = 0 and partial
 *          sums are 1 followed by the sequence terms themselves repeated in
 *          successive blocks a(0..2^k-1) for k >= 0.
 * https://oeis.org/A380855
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 2042, i16 n <= 9748, i32 n <= 9748, i64 n <= 9748,
 *            i128 n <= 9748, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A380855 – unique sequence defined by block repetition.
   a(0) = 1, a(1) = 0
   For n > 1:
       if n is a power of two:   a(n) = 1 – a((n/2) – 1)
       else:                     a(n) = a(offset) – a(offset – 1)
           where offset = n – 2^⌊log2(n)⌋ (i.e., the largest power of two
           not exceeding n).
   The cached implementation below computes terms on demand in O(n)
   total time for the first M terms, and can answer any n ≥ 0 in O(1)
   using the cache.
*/

#include <stdlib.h>
#include <stddef.h>

/* helper: highest power of two ≤ n (n > 0) */
static inline idx_t highest_power_of_two(idx_t n)
{
    idx_t p = 1;
    while ((p << 1) <= n)
        p <<= 1;
    return p;
}

seq_t a(idx_t n)
{
    static seq_t *cache     = NULL;   /* array of computed terms   */
    static size_t cache_cap = 0;      /* capacity of the array    */
    static idx_t  computed  = 1;      /* largest index computed   */

    /* initialise cache with the first two terms */
    if (cache == NULL) {
        cache_cap = 2;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
    }

    if (n <= computed)
        return cache[n];

    /* enlarge cache if necessary */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)(n + 1))
            new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute values up to n, starting after last computed index */
    for (idx_t i = computed + 1; i <= n; ++i) {
        if ((i & (i - 1)) == 0) {            /* i is a power of two */
            cache[i] = (seq_t)1 - cache[(i >> 1) - 1];
        } else {
            idx_t p = highest_power_of_two(i);
            idx_t offset = i - p;
            cache[i] = cache[offset] - cache[offset - 1];
        }
    }
    computed = n;
    return cache[n];
}
