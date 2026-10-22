/*
 * A359610  Numbers k such that the sum of the 5th powers of the digits of k
 *          is prime.
 * https://oeis.org/A359610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by brute‑force search. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* 5th powers of decimal digits 0–9.  These fit in 32 bits. */
static const uint32_t digit_p5[10] = {
    0U, 1U, 32U, 243U, 1024U, 3125U,
    7776U, 16807U, 32768U, 59049U
};

/* Small primality test for 32‑bit values – enough for all sums of fifth
   powers of decimal digits. */
static bool is_prime_u32(uint32_t n)
{
    if (n < 2U)
        return false;
    if (n % 2U == 0U)
        return n == 2U;
    for (uint32_t i = 3U; i * i <= n; i += 2U)
        if (n % i == 0U)
            return false;
    return true;
}

/* Cache of found terms.  Elements are stored as seq_t. */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;

/* Next integer to test during generation. */
static seq_t next_test = 1U;

/* Append a value to the dynamic cache array. */
static void append_to_cache(seq_t val)
{
    if (cache == NULL) {
        cache_cap = 8U;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else if (cache_len == cache_cap) {
        cache_cap *= 2U;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
    cache[cache_len++] = val;
}

/* Return the nth term of the OEIS sequence A359610
   (offset 1 – a(1) = 11).  The function generates terms on demand
   and caches them for subsequent calls. */
seq_t a(idx_t n)
{
    if (n == 0U)               /* behaviour below the offset is irrelevant */
        return 0U;

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        seq_t k  = next_test;
        uint32_t sum = 0U;
        seq_t t = k;

        /* Compute the sum of fifth powers of the digits of k. */
        do {
            uint8_t d = (uint8_t)(t % 10U);
            t /= 10U;
            sum += digit_p5[d];
        } while (t != 0U);

        if (is_prime_u32(sum))
            append_to_cache(k);

        ++next_test;
    }

    return cache[(size_t)n - 1];
}
