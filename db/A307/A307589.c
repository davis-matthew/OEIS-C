/*
 * A307589  Numbers m such that 1 < gcd(m, 35) < m and m does not divide 35^e
 *          for e >= 0.
 * https://oeis.org/A307589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A307589 by checking integers that are multiples of 5 or 7
   and contain at least one prime factor other than 5 or 7. */
#include <stddef.h>
#include "../utils/grow_seq_array.h"

static seq_t *cache = NULL;           /* cached terms */
static size_t cache_cap = 0, cache_len = 0;
static idx_t cur_m = 2;              /* next integer to test */

seq_t a(idx_t n)
{
    /* Ensure we have computed at least n terms. */
    while (cache_len < n) {
        bool ok = false;
        idx_t x = cur_m;
        /* Must share a factor 5 or 7. */
        if (x % 5 == 0 || x % 7 == 0) {
            /* Remove all 5s and 7s; if remainder 1, number is pure 5^a*7^b. */
            while (x % 5 == 0) x /= 5;
            while (x % 7 == 0) x /= 7;
            if (x != 1) ok = true;
        }

        if (ok) {
            if (cache_len == cache_cap) {
                grow_seq_array(&cache, &cache_cap, cache_len + 1);
            }
            cache[cache_len++] = (seq_t)cur_m;
        }
        cur_m++;
    }
    return cache[n - 1];
}
