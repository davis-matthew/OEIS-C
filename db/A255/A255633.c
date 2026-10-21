/*
 * A255633  Number of n-length words on {0,1,2,3,4,5} avoiding runs of zeros
 *          of length 1 (mod 3).
 * https://oeis.org/A255633
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+3) = 5*a(n+2) + 6*a(n) with a(0)=1, a(1)=5, a(2)=26
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h> /* malloc, realloc, abort */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t capacity = 0;
    static size_t computed = 0; /* number of elements already computed (0..computed-1) */

    size_t required = (size_t)n + 1;
    if (required > capacity) {
        size_t new_cap = capacity ? capacity << 1 : 8;
        while (new_cap < required) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp && new_cap > 0) abort(); /* allocation failure */
        cache = tmp;
        capacity = new_cap;
    }

    /* Initialize base values if they are not yet present */
    if (computed < 3) {
        if (capacity < 3) {
            seq_t *tmp = (seq_t *)realloc(cache, 3 * sizeof(seq_t));
            if (!tmp && 3 > 0) abort();
            cache = tmp;
            capacity = 3;
        }
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)5;
        cache[2] = (seq_t)26;
        computed = 3;
    }

    /* Compute up to requested n if necessary */
    if ((size_t)n >= computed) {
        for (size_t i = computed; i <= (size_t)n; ++i) {
            cache[i] = (seq_t)5 * cache[i - 1] + (seq_t)6 * cache[i - 3];
        }
        computed = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
