/*
 * A268165  Number of n X 2 0..3 arrays with every repeated value in every row
 *          and column one larger mod 4 than the previous repeated value, and
 *          upper left element zero.
 * https://oeis.org/A268165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Recurrence: a(n) = 23*a(n-1) - 106*a(n-2) - 724*a(n-3) + 4128*a(n-4) 
                   + 9216*a(n-5) - 20736*a(n-6);
   Initial values:
     a(1)=4, a(2)=64, a(3)=900, a(4)=12100,
     a(5)=155236, a(6)=1932100
*/

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// static cache for memoisation
static seq_t *cache = NULL;
static size_t cache_size = 0;     // number of values stored
static size_t cache_capacity = 0;

// ensure the cache can hold at least 'n' values (1‑based)
static void grow_cache(idx_t n) {
    if (n <= cache_capacity) return;
    size_t needed = (size_t)n;
    size_t new_cap = cache_capacity ? cache_capacity * 2 : 8;
    while (new_cap < needed) new_cap *= 2;
    cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    // zero‑initialize new space for safety (not strictly needed)
    memset(cache + cache_capacity, 0, (new_cap - cache_capacity) * sizeof(seq_t));
    cache_capacity = new_cap;
}

static void init_cache(void) {
    if (cache_size >= 6) return;
    grow_cache(6);
    cache[0] = (seq_t)4;
    cache[1] = (seq_t)64;
    cache[2] = (seq_t)900;
    cache[3] = (seq_t)12100;
    cache[4] = (seq_t)155236;
    cache[5] = (seq_t)1932100;
    cache_size = 6;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   // undefined; return 0
    grow_cache(n);
    init_cache();
    if (cache_size >= (size_t)n) return cache[n-1];

    // compute terms from cache_size up to n-1 (0‑based indices)
    for (size_t i = cache_size; i < (size_t)n; ++i) {
        // i>=6 because cache_size >=6
        seq_t m1 = cache[i-1];  // a(i)
        seq_t m2 = cache[i-2];  // a(i-1)
        seq_t m3 = cache[i-3];  // a(i-2)
        seq_t m4 = cache[i-4];  // a(i-3)
        seq_t m5 = cache[i-5];  // a(i-4)
        seq_t m6 = cache[i-6];  // a(i-5)

        seq_t val = (seq_t)23 * m1
                  + (seq_t)4128 * m4
                  + (seq_t)9216 * m5;
        val -= (seq_t)106 * m2;
        val -= (seq_t)724 * m3;
        val -= (seq_t)20736 * m6;

        cache[i] = val;
    }
    cache_size = n;
    return cache[n-1];
}
