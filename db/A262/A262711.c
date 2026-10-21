/*
 * A262711  Numbers k such that sum of digits of k^2 is 7.
 * https://oeis.org/A262711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth positive integer k for which the sum of the
 *            decimal digits of k² equals 7. The function searches for the
 *            required terms on demand
 * Range:     u8 n <= 7, u16 n <= 23, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Cached terms of the sequence. */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;   /* number of terms stored */
static size_t cache_cap  = 0;   /* allocated capacity of cache */

/* The next integer to test as a potential term of the sequence. */
static idx_t next_k = 1;

/* Helper: compute the sum of decimal digits of a 128‑bit unsigned integer. */
static inline uint32_t digit_sum_uint128(unsigned __int128 x)
{
    uint32_t sum = 0;
    while (x > 0) {
        sum += (uint32_t)(x % 10);
        x /= 10;
    }
    return sum;
}

/* Return the nth term of the sequence (1‑indexed). */
seq_t a(idx_t n)
{
    /* Grow the cache until it contains at least n terms. */
    while (cache_len < n) {
        /* Find the next k whose square has digit sum 7. */
        for (;;) {
            /* Prune many candidates: k² mod 9 = 7 only for k ≡ 4,5 (mod 9). */
            if (next_k % 9 != 4 && next_k % 9 != 5) {
                ++next_k;
                continue;
            }

            unsigned __int128 sq = (unsigned __int128)next_k * next_k;
            if (digit_sum_uint128(sq) == 7) {
                /* Ensure the cache has room. */
                if (cache_len >= cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) abort();        /* allocation failure */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = (seq_t)next_k;
                ++next_k;   /* advance to the next candidate for future searches */
                break;      /* exit candidate loop and possibly extend cache */
            }
            ++next_k;       /* try the next integer */
        }
    }

    /* Return the requested term. */
    return cache[(size_t)n - 1];
}
