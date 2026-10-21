/*
 * A281473  Number of 5 X n 0..1 arrays with no element equal to more than two
 *          of its horizontal, diagonal or antidiagonal neighbors and with new
 *          values introduced in order 0 sequentially upwards.
 * https://oeis.org/A281473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-3) - a(n-4) + a(n-5) - a(n-6) for
 *            n > 11 initial values: a(1)=16, a(2)=264, a(3)=133, a(4)=207,
 *            a(5)=278, a(6)=358, a(7)=472, a(8)=658, a(9)=974, a(10)=1476,
 *            a(11)=2246
 * Range:     u8 n <= 1, u16 n <= 18, u32 n <= 43, u64 n <= 92, u128 n <= 190,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t seed_values[12] = {
    /* index 0 unused */ (seq_t)0,
    (seq_t)16,  (seq_t)264,  (seq_t)133,
    (seq_t)207, (seq_t)278,  (seq_t)358,
    (seq_t)472, (seq_t)658,  (seq_t)974,
    (seq_t)1476,(seq_t)2246
};

static seq_t *cache = NULL;
static size_t cache_len = 0;   /* next index to compute */
static size_t cache_cap = 0;

/* expand cache to at least new_cap elements (including index 0) */
static void ensure_capacity(size_t new_cap)
{
    if (new_cap <= cache_cap)
        return;
    size_t cap = cache_cap ? cache_cap : 64;
    while (cap < new_cap)
        cap <<= 1;
    cache = (seq_t*)realloc(cache, (cap + 1) * sizeof(seq_t));
    if (!cache)
        exit(EXIT_FAILURE);  /* fatal error */
    cache_cap = cap;
}

seq_t a(idx_t n)
{
    if (n <= 11)
        return seed_values[(size_t)n];

    /* initialise on first use */
    if (!cache) {
        cache_cap = 64;
        cache = (seq_t*)calloc(cache_cap + 1, sizeof(seq_t));
        if (!cache)
            exit(EXIT_FAILURE);
        cache[0] = (seq_t)0;
        for (size_t i = 1; i <= 11; ++i)
            cache[(size_t)i] = seed_values[(size_t)i];
        cache_len = 12;   /* next index to compute (12) */
    }

    /* compute up to n if required */
    while (cache_len <= (size_t)n) {
        ensure_capacity(cache_len + 1);
        /* recurrence uses six previous terms */
        seq_t v =  (seq_t)2 * cache[cache_len-1]
                 - cache[cache_len-2]
                 + cache[cache_len-3]
                 - cache[cache_len-4]
                 + cache[cache_len-5]
                 - cache[cache_len-6];
        cache[cache_len] = v;
        ++cache_len;
    }

    return cache[(size_t)n];
}
