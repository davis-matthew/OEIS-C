/*
 * A366761  Numbers that have at least one exponent in their canonical prime
 *          factorization that is divisible by 3.
 * https://oeis.org/A366761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   numbers that have at least one exponent in their canonical prime
 *            factorization that is divisible by 3. Computed by trial division
 * Range:     u8 n <= 25, u16 n <= 6868, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9912 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/mem.h"

/* Returns true if a positive integer n has an exponent in its prime factorization
   that is a multiple of 3. */
static bool has_cube_exponent(seq_t n)
{
    if (n < 8)          /* the first cube is 2^3 = 8 */
        return false;

    seq_t t = n;

    /* factor 2 */
    seq_t cnt = 0;
    while ((t % 2) == 0) {
        t /= 2;
        ++cnt;
    }
    if (cnt >= 3 && cnt % 3 == 0)
        return true;

    /* odd factors */
    for (seq_t p = 3; p <= t / p; p += 2) {
        if (t % p == 0) {
            cnt = 0;
            while (t % p == 0) {
                t /= p;
                ++cnt;
            }
            if (cnt >= 3 && cnt % 3 == 0)
                return true;
        }
    }
    /* remaining prime factor has exponent 1, which is not a multiple of 3 */
    return false;
}

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache     = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t next_candidate = 1;   /* next integer to test */

    /* Generate terms until we have at least n terms cached */
    while (cache_len < (size_t)n) {
        if (has_cube_exponent(next_candidate)) {
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap ? cache_cap * 2 : 16);
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp)   /* allocation failure – unlikely for the limits used */
                    return 0;
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = next_candidate;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
