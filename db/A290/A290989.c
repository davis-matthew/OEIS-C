/*
 * A290989  Expansion of x^6*(1 + x^3)/(1 - 4*x + 5*x^2 - x^3 - 2*x^4 + x^6 +
 *          x^7 - 2*x^8 + x^9).
 * https://oeis.org/A290989
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) -5*a(n-2) + a(n-3) + 2*a(n-4) - a(n-6) - a(n-7)
 *            + 2*a(n-8) - a(n-9) for n ≥ 15. Initial values: a(6) = 1, a(7) =
 *            4, a(8) = 11, a(9) = 26, a(10) = 55, a(11) = 109, a(12) = 208,
 *            a(13) = 389, a(14) = 722. For n < 6 the function may return any
 *            value (0 is used)
 * Range:     u8 n <= 12, u16 n <= 21, u32 n <= 39, u64 n <= 74, u128 n <= 145,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache: maximum index computed so far + 1.
       Cache[0]..Cache[5] are initialized to 0, Cache[6]..Cache[14] with
       known values. Subsequent indices are filled using the recurrence. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of slots currently allocated */

    /* If cache not yet initialized, allocate and fill initial values. */
    if (cache_size == 0) {
        /* Allocate slots up to index 14 (15 slots: 0..14). */
        cache_size = 15;
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* out-of-memory guard; sequence may be invalid */

        /* Initialize indices 0..5 with 0. */
        for (idx_t i = 0; i <= 5; ++i) cache[i] = (seq_t)0;

        /* Set known initial values a(6)..a(14). */
        cache[6] = (seq_t)1;
        cache[7] = (seq_t)4;
        cache[8] = (seq_t)11;
        cache[9] = (seq_t)26;
        cache[10] = (seq_t)55;
        cache[11] = (seq_t)109;
        cache[12] = (seq_t)208;
        cache[13] = (seq_t)389;
        cache[14] = (seq_t)722;
    }

    /* Return 0 if requested n is below the offset. */
    if (n < 6) return (seq_t)0;

    /* Grow cache if necessary. */
    if (n >= cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* out-of-memory guard */
        cache = tmp;
        /* Compute new entries using the recurrence. */
        for (idx_t i = cache_size; i <= n; ++i) {
            /* All indices i-1 .. i-9 are already valid because i starts
               at cache_size which was at least 15. */
            seq_t r;
            r = (seq_t)4 * cache[i-1]
                - (seq_t)5 * cache[i-2]
                +          cache[i-3]
                + (seq_t)2 * cache[i-4]
                -          cache[i-6]
                -          cache[i-7]
                + (seq_t)2 * cache[i-8]
                -          cache[i-9];
            cache[i] = r;
        }
        cache_size = new_size;
    }

    return cache[n];
}
