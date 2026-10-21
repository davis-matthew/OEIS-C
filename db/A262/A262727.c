/*
 * A262727  Strong (2,3,5)-primes (see comments).
 * https://oeis.org/A262727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) is the n‑th strong (2,3,5)-prime. The function generates
 *            the sequence by testing each prime in increasing order and
 *            verifying the strong (2,3,5)-prime condition
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 23, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/isPrimeGeneric.h"

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static seq_t next_candidate = 2;

static void append_to_cache(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();                    /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

static bool is_strong_235_prime(seq_t p)
{
    if (!isPrimeGeneric(p))
        return false;

    /* Base‑2 digits: compute values in bases 3 and 5 on the fly. */
    unsigned bits = 0;
    seq_t temp = p;
    while (temp > 0) { ++bits; temp >>= 1; }

    seq_t val3 = 0, val5 = 0;
    for (unsigned i = 0; i < bits; ++i) {
        idx_t shift = bits - 1 - i;
        seq_t bit = (p >> shift) & 1;
        val3 = val3 * 3 + bit;
        val5 = val5 * 5 + bit;
    }
    if (!isPrimeGeneric(val3) || !isPrimeGeneric(val5))
        return false;

    /* Base‑3 digits (stored to interpret in base 5). */
    seq_t tmp = p;
    unsigned d3[256];
    unsigned len3 = 0;
    while (tmp > 0) {
        d3[len3++] = (unsigned)(tmp % 3);
        tmp /= 3;
    }
    /* len3 is always > 0 because p is prime > 1. */
    seq_t val5_3 = 0;
    for (unsigned j = len3; j-- > 0; ) {
        val5_3 = val5_3 * 5 + d3[j];
    }
    return isPrimeGeneric(val5_3);
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;    /* undefined, but return 0 for safety */

    while (cache_len < n) {
        /* Find next prime candidate. */
        while (!isPrimeGeneric(next_candidate))
            ++next_candidate;

        if (is_strong_235_prime(next_candidate))
            append_to_cache(next_candidate);

        ++next_candidate;  /* advance past the prime just examined */
    }

    return cache[n - 1];
}
