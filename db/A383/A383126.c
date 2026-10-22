/*
 * A383126  Consecutive internal states of the linear congruential
 *          pseudo-random number generator (281*s + 28411) mod 134456 when
 *          started at 1.
 * https://oeis.org/A383126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = (281*a(n-1) + 28411) mod 134456, a(1) = 1
 */
#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1 */
    if (n < 1U)
        return 0; /* undefined behaviour, but safe fallback */

    /* Initial seed value a(1) = 1 */
    if (cache_len == 0) {
        cache_cap = 4;
        cache_len = 1;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        cache[0] = 1;
    }

    while (cache_len < (size_t)n) {
        seq_t prev = cache[cache_len - 1];
        unsigned long long prod =
                (unsigned long long)281 * (unsigned long long)prev + 28411ULL;
        seq_t next = (seq_t)(prod % 134456ULL);

        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }

        cache[cache_len] = next;
        cache_len++;
    }

    return cache[n - 1];
}
