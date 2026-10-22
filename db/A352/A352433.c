/*
 * A352433  Number of tilings of a 5 X 2n rectangle using dominoes and 2 X 2
 *          tiles.
 * https://oeis.org/A352433
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 51*a(n-1) - 764*a(n-2) + 4822*a(n-3) - 13756*a(n-4) +
 *            17328*a(n-5) - 7680*a(n-6) Initial values: a(0)=1, a(1)=21,
 *            a(2)=593, a(3)=17937, a(4)=550969, a(5)=16982489
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache of already computed terms */
    static seq_t *cache = NULL;
    static size_t cached_size = 0;   /* number of elements stored */

    /* Initialise cache with the first six terms */
    if (!cached_size) {
        cache = (seq_t *)malloc(sizeof(seq_t) * 6);
        if (!cache) return (seq_t)0;  /* memory error – return 0 */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)21;
        cache[2] = (seq_t)593;
        cache[3] = (seq_t)17937;
        cache[4] = (seq_t)550969;
        cache[5] = (seq_t)16982489;
        cached_size = 6;
    }

    /* If requested term is already cached, return it */
    if ((size_t)n < cached_size) {
        return cache[(size_t)n];
    }

    /* Extend cache up to index n */
    size_t new_size = (size_t)n + 1;
    if (new_size > cached_size) {
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (!tmp) return (seq_t)0;   /* memory error – return 0 */
        cache = tmp;
        for (size_t i = cached_size; i < new_size; ++i) {
            cache[i] =
                (seq_t)51   * cache[i-1] -
                (seq_t)764  * cache[i-2] +
                (seq_t)4822 * cache[i-3] -
                (seq_t)13756* cache[i-4] +
                (seq_t)17328* cache[i-5] -
                (seq_t)7680 * cache[i-6];
        }
        cached_size = new_size;
    }

    return cache[(size_t)n];
}
