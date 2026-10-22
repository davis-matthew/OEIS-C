/*
 * A371292  Numbers whose binary indices have prime indices covering an
 *          initial interval of positive integers.
 * https://oeis.org/A371292
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 153, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A371292 – Numbers whose binary indices have prime indices
   covering an initial interval of positive integers.
   The sequence is defined as follows:
     a(n) = the nth (0‑indexed) non‑negative integer m such that
     if B(m) is the set of positions of 1‑bits in the binary
     representation of m, and for each b in B(m) let P(b) be
     the set of prime indices of b (i.e. indices i with prime(i)
     dividing b), then ⋃_{b∈B(m)} P(b) is an initial interval
     {1,2,…,k} for some k≥0.  */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Grow a dynamic array of seq_t */
static void ensure_capacity(size_t needed, size_t *cap, seq_t **arr)
{
    if (*cap < needed) {
        size_t new_cap = (*cap == 0 ? 1u : (*cap << 1));
        while (new_cap < needed) new_cap <<= 1u;
        *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

/* prime indices of integers 1…64, stored as bitmasks.
   Bit i (0‑based) is set if prime(i+1) divides the integer. */
static uint32_t prime_index_mask[65] = {0};
static bool prime_masks_initialized = false;

static void init_prime_masks(void)
{
    int primes[20];
    int pc = 0;
    for (int i = 2; i <= 64; ++i) {
        bool is_prime = true;
        for (int j = 2; j * j <= i; ++j) {
            if (i % j == 0) { is_prime = false; break; }
        }
        if (is_prime) primes[pc++] = i;
    }

    for (int n = 1; n <= 64; ++n) {
        uint32_t m = 0;
        int t = n;
        for (int pi = 0; pi < pc; ++pi) {
            int p = primes[pi];
            if (p * p > t) break;
            if (t % p == 0) {
                m |= (1u << pi);      /* prime index pi+1 */
                while (t % p == 0) t /= p;
            }
        }
        if (t > 1) {               /* remaining prime factor */
            int pi = 0;
            while (pi < pc && primes[pi] != t) ++pi;
            if (pi < pc) m |= (1u << pi);
        }
        prime_index_mask[n] = m;
    }
    prime_masks_initialized = true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t tested = 1; /* next candidate to examine */

    if (!prime_masks_initialized) init_prime_masks();

    if (cache_len == 0) {            /* initialise cache with a(0) = 0 */
        ensure_capacity(1, &cache_cap, &cache);
        cache[0] = (seq_t)0;
        cache_len = 1;
    }

    while (cache_len <= n) {
        idx_t x = tested;
        uint32_t mask = 0;

        idx_t tmp = x;
        uint8_t pos = 1;
        while (tmp) {
            if (tmp & 1u) mask |= prime_index_mask[pos];
            tmp >>= 1;
            ++pos;
        }

        if ((mask & (mask + 1u)) == 0u) {   /* mask is 2^k−1 for some k */
            ensure_capacity(cache_len + 1, &cache_cap, &cache);
            cache[cache_len++] = (seq_t)x;
        }
        ++tested;
    }

    return cache[n];
}
