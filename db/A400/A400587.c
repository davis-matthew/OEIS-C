/*
 * A400587  Numbers k such that A400531(k) = 1; k such that chi_{k,-1}(-1) =
 *          1, where chi_{k,-1} is the Dirichlet character modulo k with label
 *          -1 in the Conrey labeling system.
 * https://oeis.org/A400587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, new
 * Formula:   not stated
 * Range:     u8 n <= 123, u16 n <= 9022, u32 n <= 9022, u64 n <= 9022,
 *            u128 n <= 9022, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Sequence A400587: numbers k such that
   k is not divisible by 4 and has an even number of distinct
   prime factors congruent to 3 mod 4, or
   k is divisible by 4 and has an odd number of such prime factors. */

/* Test a single integer k for the parity condition. */
static bool passes(uint64_t k)
{
    int count = 0;
    uint64_t n = k;

    /* Remove factor 2 (does not count). */
    if (n % 2 == 0) {
        while (n % 2 == 0) n /= 2;
    }

    /* Test odd primes up to sqrt(n). */
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            if (p % 4 == 3) ++count;
            while (n % p == 0) n /= p;
        }
    }

    /* Remaining prime factor > sqrt(n). */
    if (n > 1 && n % 4 == 3) ++count;

    bool divby4 = (k & 3ULL) == 0;
    return divby4 ? ((count & 1) == 1) : ((count & 1) == 0);
}

/* Cache for the sequence. */
static seq_t *cache = NULL;          /* stored values */
static size_t cache_len = 0;         /* number of values cached */
static size_t cache_cap = 0;         /* current allocation size */
static uint64_t next_k = 1;          /* next integer to test */

/* Ensure that cache has at least n values. */
static void ensure_cache(idx_t n)
{
    while (cache_len < n) {
        if (passes(next_k)) {
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 32 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();          /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_k;
        }
        ++next_k;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for n < 1 */
    ensure_cache(n);
    return cache[n - 1];
}
