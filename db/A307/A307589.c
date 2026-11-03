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
 * Validated: standard, pass, 74 terms through n = 9910 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A307589 by checking integers that are multiples of 5 or 7
   and contain at least one prime factor other than 5 or 7. */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Helper to grow a sequence array of type seq_t. */
static void grow_seq_array(seq_t **arr, size_t *size, size_t new_len)
{
    if (*size >= new_len) return;
    size_t new_cap = (*size > 0) ? (*size << 1) : 16;
    while (new_cap < new_len) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
    if (!tmp) abort();
    *arr = tmp;
    *size = new_cap;
}

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
