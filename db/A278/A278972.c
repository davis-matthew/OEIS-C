/*
 * A278972  Twice the twin primes.
 * https://oeis.org/A278972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * p where p is the nth prime that belongs to a
 *            twin‑prime pair
 * Range:     u8 n <= 19, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t count = 0;
    static idx_t last_checked = 2;   /* start checking from 2 */

    if (n == 0) return (seq_t)0;   /* safety – the sequence is 1‑based */

    /* allocate initial cache if needed */
    if (cache == NULL) {
        cap   = 64;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) exit(EXIT_FAILURE);
    }

    /* generate terms until we have at least n of them */
    while (count < n) {
        /* advance to the next prime */
        do {
            last_checked++;
        } while (!is_prime(last_checked));

        /* is this prime part of a twin‑prime pair? */
        if (   (last_checked > 2 && is_prime(last_checked - 2))
            ||  is_prime(last_checked + 2)) {
            /* store twice the prime */
            if (count >= cap) {
                size_t new_cap = cap << 1;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                cache = tmp;
                cap   = new_cap;
            }
            cache[count++] = (seq_t)2 * (seq_t)last_checked;
        }
    }

    return cache[n - 1];
}
