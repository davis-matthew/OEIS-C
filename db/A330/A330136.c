/*
 * A330136  Numbers m such that 1 < gcd(m, 6) < m and m does not divide 6^e
 *          for e >= 0.
 * https://oeis.org/A330136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 144, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search method: iterate over integers until enough terms are found.
   A term m is accepted iff
   * 1 < gcd(m,6) < m   (equivalently, m divisible by 2 or 3 but not equal to 2,3,6)
   * m has at least one prime factor > 3   (ensures m does not divide 6^e).
   The function memoises the found terms so that repeated
   non‑sequential calls remain fast. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Cached list of terms  (1‑based). */
static seq_t *cache    = NULL;
static size_t cache_sz = 0;   /* number of terms cached */
static size_t cache_cap= 0;   /* allocated capacity of cache */
static uint64_t next_m  = 10; /* next integer to test (10 is first term) */

/* Check if an integer m satisfies the definition of A330136. */
static bool is_candidate(uint64_t m)
{
    /* Must share a non‑trivial factor with 6. */
    if (!( (m & 1ULL) == 0 || m % 3ULL == 0 ))  /* gcd(m,6) == 1 */
        return false;

    /* Must have at least one prime factor > 3. */
    uint64_t t = m;
    while ((t & 1ULL) == 0)          /* strip factors of 2 */
        t >>= 1;
    while ((t % 3ULL) == 0)          /* strip factors of 3 */
        t /= 3;
    return t > 1;                    /* some remaining factor > 3 */
}

/* Ensure that cache contains at least 'need' terms. */
static void grow_cache_to(size_t need)
{
    while (cache_sz < need) {
        /* Find the next candidate. */
        while (!is_candidate(next_m))
            ++next_m;
        /* Grow the cache if required. */
        if (cache_sz == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);           /* allocation failed */
            cache   = tmp;
            cache_cap = new_cap;
        }
        cache[cache_sz++] = (seq_t)next_m;
        ++next_m;                      /* advance to avoid re‑testing the same value */
    }
}

/* Return the n‑th term of A330136 (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* undefined for n = 0 */
    grow_cache_to((size_t)n);
    return cache[n-1];
}
