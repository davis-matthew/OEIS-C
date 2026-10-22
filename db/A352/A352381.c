/*
 * A352381  Numbers k such that no nonzero digit of 4*k divides 4*k.
 * https://oeis.org/A352381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A352381: numbers k such that no nonzero digit of 4*k divides 4*k */

#include <stdbool.h>
#include <stdlib.h>

/* Static cache for computed sequence terms */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap = 0;
/* Next integer to test as a candidate for k */
static idx_t next_k = 1;

/* Grow the cache array to hold at least 'needed' elements */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_arr;
    cache_cap = new_cap;
}

/* Check if a given k satisfies the condition */
static bool valid_k(idx_t k)
{
    seq_t m = (seq_t)k * 4;          /* 4 * k */
    seq_t temp = m;
    while (temp > 0) {
        unsigned d = (unsigned)(temp % 10);
        temp /= 10;
        if (d && ((m % d) == 0))
            return false;
    }
    return true;
}

/* Compute more terms until we have at least 'n' terms */
static void ensure_n_terms(idx_t n)
{
    grow_cache(n);
    while (cache_size < (size_t)n) {
        if (valid_k(next_k)) {
            cache[cache_size++] = next_k;
        }
        next_k++;
    }
}

seq_t a(idx_t n)
{
    /* n is 1-based; the sequence is undefined for n < 1 */
    if (n == 0)
        return 0;                 /* safety */
    ensure_n_terms(n);
    return cache[(size_t)n - 1];
}
