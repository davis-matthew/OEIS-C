/*
 * A297336  Number of nX6 0..1 arrays with every 1 horizontally or
 *          antidiagonally adjacent to 0 or 2 neighboring 1s.
 * https://oeis.org/A297336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 24*a(n-1) -154*a(n-2) +246*a(n-3) +521*a(n-4) *
 *            -1157*a(n-5) -1119*a(n-6) +1776*a(n-7) +1209*a(n-8) *
 *            -621*a(n-9) -454*a(n-10) -25*a(n-11) +10*a(n-12) +a(n-13) *
 *            (valid for n > 14) * Initial values: * a(1)=21, a(2)=333,
 *            a(3)=4888, a(4)=73147, a(5)=1091456, * a(6)=16277492,
 *            a(7)=242728633,...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* 13‑term recurrence is used only for n > 14.
 * Store the first 14 values (indices 1..14 correspond to cache[0..13]).
 */
static const seq_t init_vals[14] = {
    (seq_t)21ULL,
    (seq_t)333ULL,
    (seq_t)4888ULL,
    (seq_t)73147ULL,
    (seq_t)1091456ULL,
    (seq_t)16277492ULL,
    (seq_t)242728633ULL,
    (seq_t)3619361855ULL,
    (seq_t)53967869192ULL,
    (seq_t)804703360025ULL,
    (seq_t)11998734637184ULL,
    (seq_t)178910069329084ULL,
    (seq_t)2667681781924513ULL,
    (seq_t)39777110424677008ULL
};

/* Cache for already computed terms.
 * cache_len is the number of terms that have been computed and stored.
 */
static seq_t *cache = NULL;
static idx_t cache_len = 0; /* 0 if no terms have been computed yet */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n=0 */

    /* Allocate cache big enough for the first call or when we need more space. */
    if (!cache) {
        /* Ensure at least 14 slots to avoid frequent reallocations for small n. */
        idx_t needed = (n > 14) ? n : 14;
        cache = (seq_t *)calloc((size_t)needed, sizeof(seq_t));
        if (!cache) return 0; /* allocation failed */

        /* Copy the initial values up to the smaller of 14 and n */
        idx_t init_len = (n < 14) ? n : 14;
        for (idx_t i = 0; i < init_len; ++i) {
            cache[i] = init_vals[i];
        }
        cache_len = init_len;
    }

    /* If the requested term is already in cache, return it. */
    if (cache_len >= n) {
        return cache[n - 1];
    }

    /* Expand the cache to size n (or larger if desired) */
    if (cache_len < n) {
        idx_t new_size = n;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_size * sizeof(seq_t));
        if (!tmp) return 0; /* realloc failed; keep the old cache */
        cache = tmp;
        /* No need to initialize newly allocated tail; we will compute values. */

        /* Compute missing terms from cache_len up to n-1 (0‑based). */
        for (idx_t i = cache_len; i < n; ++i) {
            if (i < 14) {
                /* For indices 0..13, use the preset initial values. */
                cache[i] = init_vals[i];
            } else {
                seq_t term = 0;
                term += (seq_t)24   * cache[i - 1];
                term -= (seq_t)154  * cache[i - 2];
                term += (seq_t)246  * cache[i - 3];
                term += (seq_t)521  * cache[i - 4];
                term -= (seq_t)1157 * cache[i - 5];
                term -= (seq_t)1119 * cache[i - 6];
                term += (seq_t)1776 * cache[i - 7];
                term += (seq_t)1209 * cache[i - 8];
                term -= (seq_t)621  * cache[i - 9];
                term -= (seq_t)454  * cache[i -10];
                term -= (seq_t)25   * cache[i -11];
                term += (seq_t)10   * cache[i -12];
                term += cache[i -13];
                cache[i] = term;
            }
        }
        cache_len = n; /* Update the count of computed terms. */
    }

    return cache[n - 1];
}
