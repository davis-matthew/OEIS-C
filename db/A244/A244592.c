/*
 * A244592  Numbers n such that n equals the partial or complete sum of the
 *          decimal digits of 1/n, up to the point at which the digits recur
 *          or terminate.
 * https://oeis.org/A244592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A244592: numbers n such that n equals the sum of the
   decimal digits of 1/n up to (but not necessarily including) the point
   at which the decimal expansion repeats or terminates. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Test whether a given integer satisfies the definition. */
static bool is_valid(idx_t num)
{
    if (num == 1) return false;          /* 1/1 = 1.0, sum = 1, but 1 is not in the sequence */

    /* Ensure the seen array is large enough for the current remainder values. */
    static uint8_t *seen = NULL;
    static idx_t seen_cap = 0;
    idx_t needed = num + 1;               /* rem 0..num-1 plus guard */
    if (needed > seen_cap) {
        idx_t newcap = seen_cap ? seen_cap << 1 : 16;
        while (newcap < needed) newcap <<= 1;
        uint8_t *tmp = (uint8_t *)realloc(seen, newcap);
        if (!tmp) exit(1);
        seen = tmp;
        seen_cap = newcap;
    }

    /* Clear visited for this test. */
    memset(seen, 0, (size_t)needed);

    /* Simulate long division of 1/num. */
    uint64_t remainder = 1 % num;
    seen[remainder] = 1;                  /* mark starting remainder */

    seq_t sum = 0;                       /* running digit sum */

    for (;;) {
        uint64_t prod   = remainder * 10ull;
        uint64_t digit  = prod / num;
        remainder       = prod % num;

        sum += (seq_t)digit;             /* update sum with new digit */

        if (sum == (seq_t)num) return true;     /* prefix sum matches the number */
        if (sum >  (seq_t)num) return false;    /* sum exceeded target */

        if (remainder == 0) return false;       /* termination before repetition */

        if (seen[remainder]) return false;      /* first recurrence reached */
        seen[remainder] = 1;                    /* mark remainder for next iteration */
    }
}

/* Storage for the computed sequence terms. */
static seq_t *cache      = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t  next_test   = 1;   /* next integer to test for validity */

/* Ensure the cache has room for at least n elements. */
static void ensure_cache(size_t n)
{
    if (cache_cap < n) {
        size_t newcap = cache_cap ? cache_cap << 1 : 4;
        while (newcap < n) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_cap = newcap;
    }
}

/* Public API: return the nth term of the sequence (indices start at 1). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                /* guard against zero index, though unused */

    /* Generate terms until we reach the requested index. */
    while (cache_len < n) {
        if (is_valid(next_test)) {
            ensure_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)next_test;
        }
        ++next_test;
    }
    return cache[n - 1];
}
