/*
 * A281226  Primes p where largest digit of p minus sum of all other digits is
 *          equal to 1.
 * https://oeis.org/A281226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 254, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements: a(n) = nth prime p where largest digit of p minus the sum of the remaining digits equals 1 */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(idx_t) */

seq_t a(idx_t n) {
    /* static cache of discovered terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;
    static idx_t last_checked = 1;   /* last number that was examined */

    /* grow cache array if needed */
    if (n > cache_len) {
        /* Allocate or enlarge cache */
        if (cache_len == 0) {
            cache_cap = 8;
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        } else if (cache_len == cache_cap) {
            cache_cap <<= 1;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }

        /* Find additional primes until we have at least n terms */
        while (cache_len < n) {
            last_checked++;
            if (!is_prime(last_checked)) continue;

            /* digit analysis */
            idx_t tmp = last_checked;
            int sum = 0;
            int maxd = 0;
            while (tmp) {
                int d = tmp % 10;
                sum += d;
                if (d > maxd) maxd = d;
                tmp /= 10;
            }
            if (maxd - (sum - maxd) == 1) {
                /* store the prime as seq_t */
                cache[cache_len++] = (seq_t)last_checked;
            }
        }
    }
    return cache[n - 1];
}
