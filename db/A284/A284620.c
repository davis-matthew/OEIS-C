/*
 * A284620  {00->2}-transform of the infinite Fibonacci word A003849.
 * https://oeis.org/A284620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) as the {00->2}-transform of the infinite Fibonacci word (A003849). */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static seq_t *a_cache = NULL;      /* cache of computed sequence values   */
static size_t a_cap  = 0;          /* allocated capacity of a_cache      */
static idx_t  a_len  = 0;          /* number of values stored in a_cache */

static uint8_t *fib_bits = NULL;   /* current prefix of the Fibonacci word */
static size_t  fib_len  = 0;       /* length of fib_bits                  */
static idx_t   fib_idx  = 0;       /* current bit position for output     */

/* Ensure fib_bits contains at least (min_index+1) bits.  */
static void grow_fib(idx_t min_index)
{
    if (fib_bits == NULL) {
        fib_bits = (uint8_t*)malloc(1);
        fib_bits[0] = 0;
        fib_len = 1;
    }
    /* Grow until we have a bit at position min_index.             */
    while (fib_len <= (size_t)min_index) {
        size_t new_len_est = fib_len * 2 + 1;
        uint8_t *new_bits = (uint8_t*)malloc(new_len_est);
        size_t p = 0;
        for (size_t i = 0; i < fib_len; ++i) {
            if (fib_bits[i] == 0) {
                new_bits[p++] = 0;
                new_bits[p++] = 1;
            } else {
                new_bits[p++] = 0;
            }
        }
        free(fib_bits);
        fib_bits = new_bits;
        fib_len  = p;
    }
}

/* Ensure the cache has room for at least new_cap elements.          */
static void ensure_cache_cap(size_t new_cap)
{
    if (a_cap >= new_cap) return;
    size_t grow = a_cap ? a_cap * 2 : 8;
    if (grow < new_cap) grow = new_cap;
    seq_t *new_cache = (seq_t*)malloc(grow * sizeof(seq_t));
    if (a_cache) memcpy(new_cache, a_cache, a_len * sizeof(seq_t));
    free(a_cache);
    a_cache = new_cache;
    a_cap   = grow;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for offset 1, but harmless */

    /* Generate terms until we have at least n values.                */
    while (a_len < n) {
        /* Ensure we have enough bits to examine the next output.     */
        grow_fib(fib_idx + 1);     /* guarantees fib_len > fib_idx+1 */

        /* Check if the next two bits form "00".                       */
        int zero_zero = ((fib_idx + 1) < fib_len &&
                         fib_bits[fib_idx] == 0 &&
                         fib_bits[fib_idx + 1] == 0);

        ensure_cache_cap((size_t)a_len + 1);

        if (zero_zero) {
            a_cache[a_len++] = (seq_t)2;
            fib_idx += 2;
        } else {
            a_cache[a_len++] = (seq_t)fib_bits[fib_idx];
            fib_idx += 1;
        }
    }
    return a_cache[n - 1];
}
