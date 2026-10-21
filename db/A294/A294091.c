/*
 * A294091  Numbers k such that (k - 1)/2 is prime that are not congruent to
 *          -1 mod 8.
 * https://oeis.org/A294091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Numbers k such that (k-1)/2 is prime and k ≠ -1 (mod 8)
 * Range:     u8 n <= 15, u16 n <= 1747, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* That is, k = 2p+1 where p is a prime and p ≠ 3 (mod 4). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Return the n‑th element of the sequence (a(1) is first). */
    if (n == 0) return 0;  /* undefined for offset 1, but safeguards callers */

    /* Static storage for memoisation in arbitrary call order. */
    static seq_t *cache     = NULL;   /* array of computed k values */
    static size_t cache_len = 0;      /* number of values stored in cache */
    static idx_t  next_prime = 3;     /* candidate prime to test next */
    static bool   initialized = false;

    /* One‑time initialization: handle the unique case p = 2 */
    if (!initialized) {
        initialized = true;
        cache = (seq_t*)realloc(cache, sizeof(seq_t));
        if (cache == NULL) return 0;          /* allocation failure */
        cache[0] = (seq_t)2 * (seq_t)2 + 1;   /* k = 5 for p = 2 */
        cache_len = 1;
        /* next_prime already set to 3, the first odd candidate after 2. */
    }

    /* Expand cache until it contains at least n elements. */
    while (cache_len < n) {
        /* Find the next prime. */
        while (!is_prime(next_prime)) {
            next_prime++;
        }
        /* next_prime is now a prime.  We have already processed p = 2 earlier. */
        if ((next_prime & 3) == 1) {           /* p ≡ 1 (mod 4)  */
            seq_t k = (seq_t)2 * (seq_t)next_prime + 1;
            seq_t *tmp = (seq_t*)realloc(cache,
                                         (cache_len + 1) * sizeof(seq_t));
            if (tmp == NULL) return 0;         /* allocation failure */
            cache = tmp;
            cache[cache_len++] = k;
        }
        next_prime++;   /* move on to next integer candidate */
    }

    return cache[n - 1];   /* offset 1 -> stored at index 0 */
}
