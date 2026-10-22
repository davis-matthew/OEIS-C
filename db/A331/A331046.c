/*
 * A331046  Numbers k such that floor(k/10^m) is a prime number for some m >=
 *          0.
 * https://oeis.org/A331046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 163, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Series: numbers that have a prime prefix.
   The function computes terms by searching integer values
   in ascending order until the required term is found.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Simple deterministic primality test for 64‑bit integers.  */
static inline bool isPrime(uint64_t n)
{
    if (n < 2)          return false;
    if (n % 2 == 0)     return n == 2;
    if (n % 3 == 0)     return n == 3;
    for (uint64_t i = 5; i <= n / i; i += 6)
        if (n % i == 0 || n % (i + 2) == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1 */

    /* Cached results.
       cache_len: number of terms currently stored.
       cache_cap: allocated capacity of the cache array.
       next_k   : the next integer to test for membership.   */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t next_k = 2;

    /* Initialise cache lazily. */
    if (cache == NULL) {
        cache_cap = 1024;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Generate new terms until we have the nth term. */
    while (cache_len < n) {
        uint64_t tmp = next_k;
        bool ok = false;
        while (tmp > 0) {
            if (isPrime(tmp)) { ok = true; break; }
            tmp /= 10;
        }

        if (ok) {                       /* a candidate term */
            if (cache_len == cache_cap) {     /* grow cache */
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)next_k;
        }
        ++next_k;                       /* move to the next integer */
    }

    return cache[n - 1];
}
