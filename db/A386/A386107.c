/*
 * A386107  Primes having only {1, 3, 4, 7} as digits.
 * https://oeis.org/A386107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the nth prime whose decimal digits belong to {1,3,4,7}
 * Range:     u8 n <= 16, u16 n <= 205, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_cache, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/ensure_cache.h"
#include "../utils/isPrimeGeneric.h"

/* Allowed digits */
static const seq_t allowed_digits[4] = { (seq_t)1, (seq_t)3, (seq_t)4, (seq_t)7 };

/* Return the nth such prime (a(1) = 3) */
seq_t a(idx_t n)
{
    /* Cached primes found so far */
    static seq_t *cache   = NULL;
    static size_t cap     = 0;
    static idx_t  have    = 0;

    /* State for incrementally generating candidates */
    static idx_t   cur_len      = 1;   /* current number of decimal digits */
    static uint64_t idx_in_len  = 0;   /* index within the current length */
    static uint64_t limit_len   = 4;   /* 4^cur_len, upper bound for idx_in_len */

    /* Return cached result if already computed */
    if (n <= have)
        return cache[n - 1];

    /* Ensure the cache has space for at least 'n' primes */
    ensure_cache(n, &cache, &cap);

    /* Generate candidates until we find the required number of primes */
    while (have < n)
    {
        /* Build the decimal number from the base‑4 representation */
        uint64_t tmp = idx_in_len;
        seq_t val = 0;
        seq_t digits[32];
        int i = 0;

        /* Extract base‑4 digits (least significant first) */
        while (i < cur_len)
        {
            digits[i] = allowed_digits[(int)(tmp % 4)];
            tmp       /= 4;
            ++i;
        }

        /* Assemble the decimal number from most significant digit */
        for (i = cur_len - 1; i >= 0; --i)
        {
            val = val * 10 + digits[i];
        }

        /* Test primality */
        if (isPrimeGeneric(val))
        {
            cache[have++] = val;   /* Store the prime found */
        }

        /* Advance to the next candidate */
        ++idx_in_len;
        if (idx_in_len == limit_len)           /* finished this length */
        {
            idx_in_len = 0;
            ++cur_len;
            limit_len *= 4;                    /* 4^cur_len for the new length */
        }
    }

    return cache[n - 1];
}
