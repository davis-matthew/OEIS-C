/*
 * A283671  Square root of the single square referenced in A190641.
 * https://oeis.org/A283671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A249739(A190641(n)): smallest prime p with p^2 | m, where
 *            m is the n-th number having exactly one prime of exponent >= 2
 *            (found by sieve/search with caching)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 101 terms through n = 9491 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

static unsigned int *A283671_cache = NULL;
static idx_t A283671_size = 0;
static idx_t A283671_cap = 0;
static unsigned long long A283671_next = 4;

/* Returns the smallest prime p with p^2 | x if x has exactly one prime of exponent >= 2, else 0. */
static unsigned int A283671_check(unsigned long long x)
{
    unsigned int found = 0;
    unsigned long long p;
    for (p = 2; p * p <= x; ++p) {
        if (x % p == 0) {
            int e = 0;
            while (x % p == 0) { x /= p; ++e; }
            if (e >= 2) {
                if (found) return 0;
                found = (unsigned int)p;
            }
        }
    }
    return found;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    while (A283671_size < n) {
        if (A283671_size >= A283671_cap) {
            idx_t nc = A283671_cap ? A283671_cap * 2 : 4096;
            unsigned int *nb = (unsigned int *)realloc(A283671_cache, (size_t)nc * sizeof(unsigned int));
            if (!nb) exit(1);
            A283671_cache = nb;
            A283671_cap = nc;
        }
        unsigned int r = A283671_check(A283671_next);
        if (r) A283671_cache[A283671_size++] = r;
        ++A283671_next;
    }
    return (seq_t)A283671_cache[n - 1];
}
