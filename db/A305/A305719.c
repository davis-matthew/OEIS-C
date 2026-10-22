/*
 * A305719  Numbers whose squares have the same first and last digits.
 * https://oeis.org/A305719
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the n-th positive integer k such that the first decimal
 *            digit of k² equals the last decimal digit of k²
 * Range:     u8 n <= 29, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   first_digit_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/first_digit_seq_t.h"

static seq_t *cache = NULL;     /* memoised terms           */
static size_t cache_len = 0;    /* number of terms stored   */
static size_t cache_cap = 0;    /* allocated capacity of cache */
static seq_t last_k = 0;        /* last integer scanned      */

/* Compute the most significant decimal digit of a uint64_t value. */
static inline seq_t first_digit_uint64(uint64_t x)
{
    while (x >= 10U)
        x /= 10U;
    return (seq_t)x;
}

/* Ensure the cache can store at least 'min_cap' elements. */
static void grow_cache(size_t min_cap)
{
    if (cache_cap < min_cap) {
        size_t new_cap = cache_cap == 0 ? 128 : cache_cap * 2;
        if (new_cap < min_cap)
            new_cap = min_cap;
        if (cache == NULL) {
            cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        } else {
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        }
        cache_cap = new_cap;
    }
}

/* Return the n-th element of the sequence.  n >= 1.           */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* offset starts at 1 */

    /* initialise memoisation state on first call */
    if (cache == NULL) {
        grow_cache(128);
        cache_len = 0;
        last_k = 0;
    }

    /* enlarge cache so address calculations are valid later       */
    if (n > (idx_t)cache_cap)
        grow_cache((size_t)n);

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        ++last_k;

        seq_t first_digit;
        seq_t last_digit;

        /* choose computation method based on seq_t size          */
        if (sizeof(seq_t) <= 8) {
            /* intermediate 64‑bit arithmetic avoids overflow   */
            uint64_t sq64 = (uint64_t)last_k * (uint64_t)last_k;
            last_digit = (seq_t)(sq64 % 10U);
            first_digit = first_digit_uint64(sq64);
        } else {
            /* use seq_t multiplication for arbitrary‑precision  */
            seq_t sq = last_k * last_k;
            last_digit = sq % (seq_t)10;
            first_digit = first_digit_seq_t(sq);
        }

        if (first_digit == last_digit)
            cache[cache_len++] = last_k;
    }

    return cache[n - 1];
}
