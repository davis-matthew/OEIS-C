/*
 * A386155  Primes having only {2, 4, 6, 7} as digits.
 * https://oeis.org/A386155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 61, u32 n <= 228, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A386155:
 * primes whose decimal representation uses only the digits {2,4,6,7}. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n) */

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *cache = NULL;          /* array of found primes */
    static size_t cache_cap = 0;          /* allocated capacity of cache   */
    static idx_t found = 0;              /* number of primes stored      */
    static int cur_len = 0;              /* current number of digits     */
    static int digits[128];              /* indices in [0..3] for digits */

    /* mapping from index to actual decimal digit */
    static const int digit_map[4] = { 2, 4, 6, 7 };

    /* initialise on first call */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cur_len = 1;
        for (int i = 0; i < cur_len; ++i) digits[i] = 0;
    }

    /* produce primes until we have at least n of them */
    while (found < n) {
        /* build the current candidate number */
        seq_t val = 0;
        for (int i = 0; i < cur_len; ++i) {
            val = val * 10 + (seq_t)digit_map[digits[i]];
        }

        /* test primality */
        if (is_prime_big(val)) {
            /* store into cache, growing if necessary */
            if ((size_t)found >= cache_cap) {
                cache_cap <<= 1;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[found++] = val;
        }

        /* advance to the next candidate in base‑4 order */
        int pos = cur_len - 1;
        while (pos >= 0) {
            if (digits[pos] < 3) {
                ++digits[pos];
                break;
            } else {
                digits[pos] = 0;
                --pos;
            }
        }
        if (pos < 0) {              /* exhausted all numbers of this length */
            ++cur_len;
            for (int i = 0; i < cur_len; ++i) digits[i] = 0;
        }
    }

    /* return the requested term */
    return cache[n - 1];
}
