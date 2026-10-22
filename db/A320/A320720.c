/*
 * A320720  Indices of primes followed by a gap (distance to next larger
 *          prime) of 44.
 * https://oeis.org/A320720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = index of the n-th prime p such that the next prime
 *            following p is exactly 44 larger than p. Equivalently a(n) =
 *            A000720(A134121(n))
 * Range:     u8 none, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   ensure_capacity, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/next_prime.h"          /* provides uint64_t next_prime(uint64_t) */
#include "../utils/ensure_capacity.h"    /* provides void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size) */

static seq_t *indices_cache = NULL;       /* cached indices of qualifying primes */
static size_t cache_cap   = 0;            /* capacity of the cache array */
static size_t cache_len   = 0;            /* number of cached indices */
static uint64_t prev_prime = 0;           /* previous prime examined */
static idx_t curr_idx      = 0;           /* index of prev_prime in the prime sequence */
static bool initialized   = false;      /* whether the state has been initialized */

/* Extends the cache if necessary and returns a pointer to its storage. */
static void extend_cache(void) {
    size_t needed = cache_len + 1;
    ensure_capacity(needed, &cache_cap, (void **)&indices_cache, sizeof(seq_t));
}

/* Precomputes prime indices until at least n entries are stored in the cache. */
static void precompute_to(idx_t n) {
    if (!initialized) {
        /* initialise at the first prime 2 */
        prev_prime = 2u;
        curr_idx   = 1u;          /* index of prime 2 */
        initialized = true;
    }

    while (cache_len < n) {
        uint64_t next = next_prime(prev_prime); /* next prime after prev_prime */
        /* If the gap to the next prime is 44, store the current index. */
        if ((uint64_t)(next - prev_prime) == 44u) {
            extend_cache();
            indices_cache[cache_len++] = (seq_t)curr_idx;
        }
        /* Advance to the next prime in the sequence. */
        prev_prime = next;
        curr_idx++;  /* index of the new current prime */
    }
}

seq_t a(idx_t n) {
    if (n < 1) return 0;   /* undefined for n == 0, return 0 safely */
    precompute_to(n);
    return indices_cache[n - 1];
}
