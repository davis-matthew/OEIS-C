/*
 * A336234  Edge length of 'Prime squares': sum the four numbers at the
 *          corners of a square drawn on a diagonally numbered 2D board, with
 *          1 at the corner of the square. The sequence gives the size of the
 *          square such that the sum is a prime number.
 * https://oeis.org/A336234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th odd integer d such that 3*d*d + 4*d + 4 is a
 *            prime
 * Range:     u8 n <= 3, u16 n <= 21, u32 n <= 24, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A336234.  See the above comment for the
   exact definition.  The function is memoised and works for any
   sequence value that fits in seq_t; it does not assume increasing
   order of calls.  */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

/* Static cache of computed d values. */
static seq_t *cache = NULL;      /* pointer to cached values */
static size_t cache_cap = 0;     /* current allocated capacity */
static size_t cache_len = 0;     /* number of values stored */
static idx_t next_d = 1;         /* next side length to test */

/* Ensure the cache can hold at least 'need' values. */
static void grow_cache(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_ptr;
    cache_cap = new_cap;
}

/* Find the next d such that 3*d^2+4*d+4 is prime, store it, and
   advance next_d. */
static void extend_cache(void)
{
    for (;;) {
        unsigned long long d = next_d;
        unsigned long long sum = 3ULL * d * d + 4ULL * d + 4ULL;
        seq_t s = (seq_t)sum;
        if (is_prime_big(s)) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)next_d;
            next_d++;                 /* advance to next candidate */
            break;
        }
        next_d++;                     /* not prime – try next d */
    }
}

seq_t a(idx_t n)
{
    /* Ensure we have computed at least n terms. */
    while (cache_len < (size_t)n)
        extend_cache();
    return cache[n - 1];   /* sequence is 1‑based indexed */
}
