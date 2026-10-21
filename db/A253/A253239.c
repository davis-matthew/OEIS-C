/*
 * A253239  Numbers k such that k^2 + k + 72491 is prime.
 * https://oeis.org/A253239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 4534, u128 n <= 4534,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence computed by testing k^2 + k + 72491 for primality
   (OEIS A253239: Numbers k such that k^2 + k + 72491 is prime). */
#include <stdlib.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* static cache of found k values */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;
    static idx_t  next_k = 1;

    /* search until we have at least n terms */
    while (len < n) {
        seq_t kk   = (seq_t)next_k;
        seq_t expr = kk * kk + kk + (seq_t)72491;

        if (is_prime_big(expr)) {
            if (len + 1 > cap) {
                size_t new_cap = cap ? cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = tmp;
                cap   = new_cap;
            }
            cache[len++] = next_k;
        }
        ++next_k;
    }
    return cache[n - 1];
}
