/*
 * A375595  Numbers m for which the sum of all values of k satisfying the
 *          equation: m mod k = floor((m - k)/k) mod k (1 <= k <= m) exceeds
 *          2*m.
 * https://oeis.org/A375595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements a(n) = nth m such that the sum of all k (1 ≤ k ≤ m) with
   m mod k = floor((m‑k)/k) mod k exceeds 2*m. This follows from the
   OEIS definition of A375595. */

#include <stdlib.h>

static seq_t *cache = NULL;          /* stored valid m's */
static size_t cache_len = 0;        /* number of entries stored */
static size_t cache_cap = 0;        /* allocated capacity for cache */
static idx_t current_m = 0;         /* last m tested */

/* Append a new m to the cache, growing the array if needed */
static void append_cache(idx_t m)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
    cache[cache_len++] = (seq_t) m;
}

/* Compute the nth term of sequence A375595 */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for offset 1, but safe for guard */

    /* return cached value if already computed */
    if (n <= cache_len)
        return cache[n - 1];

    /* continue searching for new terms until we reach the desired index */
    while (cache_len < (size_t)n) {
        ++current_m;                 /* next m to test */
        seq_t sum = 0;               /* sum of qualifying k's */
        seq_t Mplus1 = (seq_t)current_m + 1;
        /* For each k from 1 to current_m evaluate the condition.
           The test is: floor(current_m/k) % k == (current_m+1) % k      */
        for (idx_t k = 1; k <= current_m; ++k) {
            idx_t q = current_m / k;                     /* floor division */
            if ((q % k) == ((Mplus1) % k))
                sum += (seq_t)k;
            /* early exit: once sum exceeds 2*m we know this m qualifies */
            if (sum > (seq_t)(2 * current_m))
                break;
        }
        if (sum > (seq_t)(2 * current_m))
            append_cache(current_m);
    }

    return cache[n - 1];
}
