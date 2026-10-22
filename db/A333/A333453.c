/*
 * A333453  Binary concatenation (ignoring leading zeros) of a(n-1) and a(n-2)
 *          mod n, starting with a(n) = n for n <= 1.
 * https://oeis.org/A333453
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   * a(0) = 0, a(1) = 1 * for n > 1: * a(n) =
 *            (a(n-1)*A062383(a(n-2)) + a(n-2)) mod n * where A062383(k) = bit
 *            length of k in binary (0 if k = 0)
 * Range:     u8 n <= 211, u16 n <= 9257, u32 n <= 9257, u64 n <= 9257,
 *            u128 n <= 9257, big any n
 * Helpers:   bitlen_seq_t, powmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/bitlen_seq_t.h"
#include "../utils/powmod64.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated capacity of cache */
static idx_t  cache_next = 0;  /* first index not yet computed */

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Base cases for n = 0 or 1 (no need to use the cache). */
    if (n < 2)
        return (seq_t)n;

    /* Initialize cache for the trivial first two values. */
    if (cache_next == 0) {
        cache_cap = 2;
        cache    = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = 0;
        cache[1] = 1;
        cache_next = 2;
    }

    /* Grow the cache if necessary. */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 2;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing terms up to n. */
    for (size_t i = cache_next; i <= (size_t)n; ++i) {
        seq_t a_prev  = cache[i - 1];
        seq_t a_prev2 = cache[i - 2];

        /* Number of bits in a_prev2 (ignoring leading zeros). */
        unsigned bl = bitlen_seq_t(a_prev2);

        /* Compute 2^bl (mod i) using the supplied modular exponent. */
        uint64_t pow2 = powmod64(2, bl, (uint64_t)i);

        /* Combine according to the recurrence and reduce modulo i. */
        seq_t term = (a_prev * (seq_t)pow2 + a_prev2) % (seq_t)i;
        cache[i] = term;
    }

    cache_next = (size_t)n + 1;
    return cache[n];
}
