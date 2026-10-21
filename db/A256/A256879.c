/*
 * A256879  Numbers divisible by prime(d) for each digit d in their base-9
 *          representation, none of which may be zero.
 * https://oeis.org/A256879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Numbers divisible by prime(d) for each digit d in base-9
 *            representation, no digit zero
 * Range:     u8 n <= 3, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* prime for each non‑zero base‑9 digit d (1≤d≤8) */
static const uint8_t prime_for_digit[9] = {0,2,3,5,7,11,13,17,19};

/* test whether x satisfies the property */
static inline int satisfies(idx_t x)
{
    idx_t t = x;
    while (t > 0) {
        int d = t % 9;
        if (d == 0) return 0;                    /* zero digit not allowed */
        uint64_t p = prime_for_digit[d];
        if (x % p != 0) return 0;                /* not divisible by required prime */
        t /= 9;
    }
    return 1;
}

/* a(n) -- nth term of A256879 for any order of calls */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_len = 0;
    static idx_t candidate = 1;                 /* next integer to test */

    if (cache_len >= n) return cache[n-1];

    if (cache_cap == 0) {                       /* initial allocation */
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    }

    while (cache_len < n) {
        while (!satisfies(candidate)) candidate++;

        if (cache_len >= cache_cap) {           /* grow the cache if needed */
            cache_cap *= 2;
            cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
        }

        cache[cache_len++] = (seq_t)candidate;  /* store the found term */
        candidate++;                            /* advance to next candidate */
    }

    return cache[n-1];
}
