/*
 * A373466  Palindromes with exactly 6 distinct prime divisors.
 * https://oeis.org/A373466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A373466: Palindromes with exactly 6 distinct prime divisors */
/* Implementation: generate decimal palindromes in order, test each with
   the utility count_distinct_primes, cache the first n satisfying
   the condition.  Works for any seq_t that can hold a uint64_t value. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/count_distinct_primes.h"

/* Powers of 10 up to 10^19 – sufficient for 64‑bit palindrome construction. */
static const uint64_t pow10[20] = {
    1ULL, 10ULL, 100ULL, 1000ULL, 10000ULL, 100000ULL,
    1000000ULL, 10000000ULL, 100000000ULL, 1000000000ULL,
    10000000000ULL, 100000000000ULL, 1000000000000ULL,
    10000000000000ULL, 100000000000000ULL, 1000000000000000ULL,
    10000000000000000ULL, 100000000000000000ULL,
    1000000000000000000ULL, 10000000000000000000ULL
};

/* Generator state for decimal palindromes. */
static uint8_t  pal_len   = 1;          /* current palindrome length       */
static uint64_t half_val  = 0;          /* current half value              */
static uint64_t half_lo   = 0;          /* lowest half for current length  */
static uint64_t half_hi   = 9;          /* highest half for current length */
static int      init_done = 0;          /* flag indicating initialization  */

/* Produce the next decimal palindrome in increasing order. */
static uint64_t next_palindrome(void)
{
    if (!init_done) {
        pal_len  = 1;
        half_lo  = 0;
        half_hi  = 9;
        half_val = half_lo;
        init_done = 1;
    }

    /* Build palindrome from current half value. */
    uint8_t half_len = (pal_len + 1) / 2;
    uint64_t rev = 0, tmp = half_val;
    if (pal_len & 1) tmp /= 10;          /* skip middle digit for odd length */
    while (tmp > 0) {
        rev = rev * 10 + (tmp % 10);
        tmp /= 10;
    }
    uint64_t mult = pow10[half_len - (pal_len & 1)];
    uint64_t pal  = half_val * mult + rev;

    /* Advance generator state. */
    half_val++;
    if (half_val > half_hi) {
        pal_len++;
        half_len = (pal_len + 1) / 2;
        half_lo = (pal_len == 1) ? 0 : pow10[half_len - 1];
        half_hi = pow10[half_len] - 1;
        half_val = half_lo;
    }

    return pal;
}

/* Cache of computed sequence values. */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;

/* Retrieve the nth term (1‑based) of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* sequence is defined for n ≥ 1 */

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        while (1) {
            uint64_t cand = next_palindrome();
            if (cand < 2) continue;                /* skip 0,1 */

            if (count_distinct_primes(cand) == 6) {
                /* Ensure capacity. */
                if (cache_len >= cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) return 0; /* allocation failure */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = (seq_t)cand;
                break; /* go to next required term */
            }
        }
    }

    return cache[n - 1];
}
