/*
 * A262838  {3,7}-primes (defined in Comments).
 * https://oeis.org/A262838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 15, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_capacity, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Finds the n‑th {3,7}-prime: a prime p such that the digits of p in base
   3 spell a prime in base 7 and vice versa.  The sequence is stored in a
   dynamic cache for repeated calls. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/ensure_capacity.h"

/* Reinterpret the digits of a number written in base 'from'
   as a number written in base 'to'. */
static inline seq_t convert_base(seq_t value, unsigned from, unsigned to)
{
    seq_t digits[128];
    size_t len = 0;
    seq_t v = value;

    if (v == 0) return 0;
    while (v > 0) {
        digits[len++] = v % from;
        v = v / from;
    }

    seq_t res = 0;
    for (size_t i = len; i > 0; --i) {
        res = res * to + digits[i - 1];
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Cache of already found terms. */
    static seq_t *cache = NULL;
    static size_t  cache_cap = 0;
    static size_t  cache_len = 0;
    static idx_t   next_candidate = 2;

    while (cache_len < n) {
        /* Find next prime candidate. */
        seq_t p;
        do {
            p = (seq_t)next_candidate;
            ++next_candidate;
        } while (!is_prime_big(p));

        /* Check base-3 → base-7 conversion. */
        seq_t v3to7 = convert_base(p, 3, 7);
        if (!is_prime_big(v3to7))
            continue;

        /* Check base-7 → base-3 conversion. */
        seq_t v7to3 = convert_base(p, 7, 3);
        if (!is_prime_big(v7to3))
            continue;

        /* Store the {3,7}-prime. */
        size_t needed = cache_len + 1;
        ensure_capacity(needed, &cache_cap, (void **)&cache, sizeof(seq_t));
        cache[cache_len++] = p;
    }

    return cache[n - 1];
}
