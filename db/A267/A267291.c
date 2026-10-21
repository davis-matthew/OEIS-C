/*
 * A267291  Primes which are at 2/3 of the distance between their neighbors.
 * https://oeis.org/A267291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: generate primes in increasing order and keep those for which
   the left gap equals twice the right gap, i.e. p - prev = 2*(next - p). */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/next_prime.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_len = 0;

static bool state_initialized = false;
static uint64_t state_prev;
static uint64_t state_cur;
static uint64_t state_next;

static void ensure_cache_size(idx_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < (size_t)needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();  /* fatal error if allocation fails */
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset starts at 1 */
    if (cache_len >= n) return cache[n - 1];

    if (!state_initialized) {
        state_prev = 2;
        state_cur  = 3;
        state_next = next_prime(state_cur + 1);
        state_initialized = true;
    }

    while (cache_len < n) {
        if ((state_cur - state_prev) == 2 * (state_next - state_cur)) {
            ensure_cache_size(cache_len + 1);
            cache[cache_len++] = (seq_t)state_cur;
        }
        state_prev = state_cur;
        state_cur  = state_next;
        state_next = next_prime(state_cur + 1);
    }

    return cache[n - 1];
}
