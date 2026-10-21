/*
 * A264315  Number of (2+1)X(n+1) arrays of permutations of 0..n*3+2 with each
 *          element having directed index change 0,0 0,1 1,0 -1,-2 or 0,2.
 * https://oeis.org/A264315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence for A264315 (offset 1):
   a(1) = 1, a(2)=20, a(3)=139, a(4)=735, a(5)=4103, a(6)=24153,
   a(7)=137703, a(8)=781453, a(9)=4441735, a(10)=25301221,
   a(11)=144075295.
   For n ≥ 12:
   a(n) = 3*a(n-1)+10*a(n-2)+18*a(n-3)+72*a(n-4)+40*a(n-5)
          -152*a(n-6)-728*a(n-7)-400*a(n-8)
          +1008*a(n-9)+704*a(n-10)+192*a(n-11).  */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* cache[0] is unused; cache[i] holds a(i) for 1 ≤ i ≤ cache_len */
static seq_t *cache = NULL;
static idx_t cache_len = 0;

static const seq_t init_vals[12] = {
    0ULL,          /* dummy for index 0 */
    1ULL, 20ULL, 139ULL, 735ULL, 4103ULL,
    24153ULL, 137703ULL, 781453ULL, 4441735ULL,
    25301221ULL, 144075295ULL
};

/* Allocate or grow cache up to at least index n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (n <= cache_len)
        return;

    /* New size must accommodate indices 0..n */
    size_t new_size = (size_t)(n + 1);
    if (cache == NULL) {
        cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (cache == NULL)
            abort();                  /* memory exhaustion */
    } else {
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL)
            abort();
        cache = tmp;
    }

    /* Initialize up to the highest initial value that we have. */
    const size_t init_limit = 11;
    size_t start = cache_len + 1;
    if (start < 1)
        start = 1;
    for (size_t i = start; i <= init_limit && i <= (size_t)n; ++i)
        cache[i] = init_vals[i];

    /* Compute subsequent values using the empirical recurrence. */
    for (size_t i = init_limit + 1; i <= (size_t)n; ++i) {
        seq_t value;
        if (sizeof(seq_t) <= 8) {          /* built‑in integral types */
            unsigned __int128 v =
                3ULL * (unsigned __int128)cache[i-1] +
                10ULL * (unsigned __int128)cache[i-2] +
                18ULL * (unsigned __int128)cache[i-3] +
                72ULL * (unsigned __int128)cache[i-4] +
                40ULL * (unsigned __int128)cache[i-5];

            v -= 152ULL * (unsigned __int128)cache[i-6] +
                 728ULL * (unsigned __int128)cache[i-7] +
                 400ULL * (unsigned __int128)cache[i-8];

            v += 1008ULL * (unsigned __int128)cache[i-9] +
                 704ULL * (unsigned __int128)cache[i-10] +
                 192ULL * (unsigned __int128)cache[i-11];

            value = (seq_t)v;               /* truncate to seq_t bits */
        } else {                            /* arbitrary‑precision types */
            value =
              3 * cache[i-1] + 10 * cache[i-2] + 18 * cache[i-3] +
              72 * cache[i-4] + 40 * cache[i-5]
            - 152 * cache[i-6] - 728 * cache[i-7] - 400 * cache[i-8]
            + 1008 * cache[i-9] + 704 * cache[i-10] + 192 * cache[i-11];
        }
        cache[i] = value;
    }

    cache_len = n;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;            /* undefined for non‑positive indices */
    ensure_cache(n);
    return cache[n];
}
