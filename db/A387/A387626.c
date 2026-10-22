/*
 * A387626  a(n) = Sum_{k=0..floor(n/4)} 2^k * binomial(2*n-6*k+1,2*k).
 * https://oeis.org/A387626
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/4)} 2^k * binomial(2*n-6*k+1, 2*k)
 *            Recurrence (for n ≥ 8): a(n) = 2*a(n‑1) - a(n‑2) + 4*a(n‑4) +
 *            4*a(n‑5) - 4*a(n‑8) Base values: a(0)=1, a(1)=1, a(2)=1, a(3)=1,
 *            a(4)=7, a(5)=21, a(6)=43, a(7)=73
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 30, u64 n <= 60, u128 n <= 119,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;      /* cache[0..cache_count-1] */
static size_t cache_count = 0;   /* number of elements stored */

/* Grow the cache to hold values up to n (inclusive).  All values are
   computed using the recurrence after the initial 8 terms. */
static void ensure_cache(idx_t n)
{
    if (cache_count <= (size_t)n) {
        size_t new_count = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_count * sizeof(seq_t));
        if (!tmp) abort();                /* allocation failed */
        cache = tmp;
        for (size_t i = cache_count; i < new_count; ++i) {
            if (i <= 3) {
                cache[i] = (seq_t)1;
            } else if (i == 4) {
                cache[i] = (seq_t)7;
            } else if (i == 5) {
                cache[i] = (seq_t)21;
            } else if (i == 6) {
                cache[i] = (seq_t)43;
            } else if (i == 7) {
                cache[i] = (seq_t)73;
            } else {
                /* recurrence: a(i) = 2*a(i-1) - a(i-2) + 4*a(i-4) + 4*a(i-5) - 4*a(i-8) */
                seq_t val = (seq_t)2 * cache[i-1];
                val -= cache[i-2];
                val += (seq_t)4 * cache[i-4];
                val += (seq_t)4 * cache[i-5];
                val -= (seq_t)4 * cache[i-8];
                cache[i] = val;
            }
        }
        cache_count = new_count;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
