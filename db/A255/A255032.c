/*
 * A255032  Number of (n+2)X(4+2) 0..1 arrays with every 3X3 subblock sum of
 *          the two sums of the central row and central column minus the two
 *          minimums of the diagonal and antidiagonal nondecreasing
 *          horizontally, vertically and ne-to-sw antidiagonally.
 * https://oeis.org/A255032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 42, u64 n <= 106, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence:
   a(n) = a(n-1)+a(n-2)-a(n-3)+4*a(n-4)-3*a(n-5)-5*a(n-6)+3*a(n-7)
          +a(n-8)-4*a(n-9)+4*a(n-10)+4*a(n-11)-4*a(n-12)  for n > 20
   Initial values a(1)…a(20) are given below. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* cache[1] corresponds to a(1); cache[0] is unused */
static seq_t *cache = NULL;
static idx_t cache_size = 0;     /* maximum index stored (cache[cache_size] allocated) */
static idx_t computed_up_to = 0; /* largest n for which cache[n] is computed */
static bool cache_ready   = false;  /* set when a(1)..a(20) are filled */

/* ensure cache has room for indices up to n (inclusive) */
static void ensure_cache_size(idx_t n)
{
    if (n <= cache_size) return;
    idx_t new_size = (cache_size == 0) ? 32 : cache_size * 2;
    while (new_size < n) new_size <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, (size_t)(new_size + 1) * sizeof(seq_t));
    cache = tmp;
    cache_size = new_size;
}

/* first 20 terms of the sequence */
static const seq_t init_vals[20] = {
    42890ULL, 149676ULL, 246173ULL, 281112ULL, 340084ULL,
    301745ULL, 310872ULL, 321641ULL, 365650ULL, 388347ULL,
    456154ULL, 520919ULL, 607758ULL, 664717ULL, 850912ULL,
    1044341ULL, 1325468ULL, 1478889ULL, 2146514ULL, 2860091ULL
};

seq_t a(idx_t n)
{
    if (n < 1) return 0;              /* undefined for n < 1 */

    ensure_cache_size(n);

    if (!cache_ready) {
        /* initialise the first 20 values once */
        for (idx_t i = 1; i <= 20; ++i) cache[i] = init_vals[i-1];
        cache_ready = true;
        computed_up_to = 20;
    }

    if (n <= computed_up_to) return cache[n];

    /* compute terms only for indices that have not been computed yet */
    for (idx_t i = computed_up_to + 1; i <= n; ++i) {
        /* i > 20 by construction */
        seq_t pos = cache[i-1] + cache[i-2];
        pos += 4u * cache[i-4];
        pos += 3u * cache[i-7];
        pos += cache[i-8];          /* + a(n-8) */
        pos += 4u * cache[i-10];
        pos += 4u * cache[i-11];

        seq_t neg = cache[i-3];
        neg += 3u * cache[i-5];
        neg += 5u * cache[i-6];
        neg += 4u * cache[i-9];
        neg += 4u * cache[i-12];

        cache[i] = pos - neg;
    }

    computed_up_to = n;
    return cache[n];
}
