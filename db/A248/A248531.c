/*
 * A248531  Numbers k such that the smallest prime divisor of k^2+1 is 41.
 * https://oeis.org/A248531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth integer k>0 such that the smallest prime divisor
 *            of k²+1 is 41. Candidates must satisfy k≡32 or 50 (mod 82); the
 *            rest is tested by checking divisibility by each prime < 41
 * Range:     u8 n <= 4, u16 n <= 633, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/* Dynamic array growth helper. */
static void grow_array(seq_t **arr, size_t *cap, size_t need)
{
    if (need <= *cap) return;
    size_t new_cap = (*cap > 0) ? (*cap << 1) : 8;
    if (new_cap < need) new_cap = need;
    *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    if (!*arr) abort();
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Small primes that could divide k²+1 before 41.  All are < 41. */
    static const idx_t SMALL_PRIMES[] = {5, 13, 17, 29, 37};
    static const size_t SMALL_PRIME_COUNT =
        sizeof(SMALL_PRIMES) / sizeof(SMALL_PRIMES[0]);

    /* State for memoization over arbitrary order of calls. */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t  last_k   = 0;   /* largest k already examined */

    /* Initialise static data on first call. */
    if (!cache) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
    }

    /* Produce terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        for (;;) {
            ++last_k;
            /* Only k ≡ 32 or 50 (mod 82) can have smallest prime 41. */
            idx_t mod82 = last_k % 82;
            if (mod82 != 32 && mod82 != 50) continue;

            bool ok = true;
            for (size_t i = 0; i < SMALL_PRIME_COUNT && ok; ++i) {
                idx_t p = SMALL_PRIMES[i];
                idx_t r = last_k % p;
                /* If (k²+1) ≡ 0 (mod p), reject. */
                if (((r * r) + 1) % p == 0) ok = false;
            }
            if (!ok) continue;

            /* Accept this k.  Grow cache if needed. */
            if (cache_len == cache_cap)
                grow_array(&cache, &cache_cap, cache_len + 1);

            cache[cache_len++] = (seq_t) last_k;
            break;  /* next candidate */
        }
    }

    return cache[n - 1];
}
