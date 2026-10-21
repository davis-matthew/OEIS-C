/*
 * A281093  Primes having only {3, 4, 7, 9} as digits.
 * https://oeis.org/A281093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = n-th prime whose decimal digits belong only to {3,4,7,9}
 * Range:     u8 n <= 8, u16 n <= 165, u32 n <= 50, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"

/* Check that all decimal digits of x belong to the allowed set. */
static inline bool allowed_digits(uint64_t x)
{
    while (x != 0) {
        uint8_t d = (uint8_t)(x % 10);
        if (d != 3 && d != 4 && d != 7 && d != 9)
            return false;
        x /= 10;
    }
    return true;
}

/* Cache of found terms. */
static seq_t *seq_cache = NULL;
static size_t   seq_cap  = 0;
static size_t   seq_len  = 0;
static uint64_t next_candid = 3;

/* Ensure the cache can hold at least new_cap elements. */
static void grow_cache(size_t new_cap)
{
    if (seq_cap >= new_cap)
        return;
    size_t cap = seq_cap == 0 ? 64 : seq_cap * 2;
    while (cap < new_cap)
        cap *= 2;
    seq_t *tmp = (seq_t *)realloc(seq_cache, cap * sizeof(seq_t));
    if (!tmp)
        abort();
    seq_cache = tmp;
    seq_cap   = cap;
}

/* Main function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for n==0, caller should not use this */

    while (seq_len < (size_t)n) {
        if (allowed_digits(next_candid) && is_prime_big((seq_t)next_candid)) {
            grow_cache(seq_len + 1);
            seq_cache[seq_len++] = (seq_t)next_candid;
        }
        /* Move to the next odd candidate. */
        if (next_candid == 2)
            next_candid = 3;
        else
            next_candid += 2;   /* only odd numbers */
    }
    return seq_cache[(size_t)n - 1];
}
