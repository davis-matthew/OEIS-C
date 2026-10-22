/*
 * A386322  Primes without {0, 4} as digits.
 * https://oeis.org/A386322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 2924, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th prime whose decimal digits are
   all non‑zero and non‑four (the OEIS A386322 sequence). */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(idx_t) */

/* --- static cache for memoisation --------------------------------------- */
static seq_t *cache       = NULL;   /* stored primes           */
static idx_t cache_cap    = 0;      /* allocated size in entries */
static idx_t cache_len    = 0;      /* number of primes stored   */
static idx_t current      = 2;      /* next integer to test     */

/* --- check that a number contains only the allowed digits 1,2,3,5,6,7,8,9 */
static inline bool allowed_digits(idx_t num)
{
    while (num > 0) {
        int d = num % 10;
        if (d == 0 || d == 4)
            return false;
        num /= 10;
    }
    return true;
}

/* --- grow the cache if a larger capacity is required -------------------- */
static void grow_cache(idx_t min_cap)
{
    if (cache_cap >= min_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < min_cap)
        new_cap = min_cap;

    seq_t *new_buf = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_buf)
        abort();   /* allocation failure: terminate */

    if (cache) {
        memcpy(new_buf, cache, cache_len * sizeof(seq_t));
        free(cache);
    }

    cache     = new_buf;
    cache_cap = new_cap;
}

/* --- a(n) – the n‑th prime that avoids digits 0 and 4 ------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* sequence is 1‑based */

    /* ensure the cache contains at least n primes */
    if (cache_len < n) {
        grow_cache(n);
        for ( ; cache_len < n; ++current) {
            if (is_prime(current) && allowed_digits(current)) {
                cache[cache_len++] = (seq_t)current;
            }
        }
    }
    return cache[n - 1];
}
