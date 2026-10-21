/*
 * A245802  Numbers that are divisible by the sum of their base 8 digits.
 * https://oeis.org/A245802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 7664, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A245802: a(n) = the n‑th positive integer k such that
   k modulo the sum of its base‑8 digits equals zero.  The function
   builds the sequence incrementally, caching results for any order
   of calls. */
#include <stdlib.h>

static seq_t *seq_cache   = NULL;   /* cached terms                     */
static size_t   cache_len = 0;      /* number of terms cached          */
static size_t   cache_cap = 0;      /* allocated capacity of seq_cache */
static seq_t   candidate   = 1;     /* next integer to test            */

/* ensure seq_cache can hold at least `needed` elements */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    if (new_cap < needed)
        new_cap = needed;
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) /* undefined, but return 0 for safety */
        return 0;

    /* extend cache until it contains the n‑th term */
    while (cache_len < (size_t)n) {
        /* compute sum of octal digits of `candidate` */
        seq_t tmp = candidate;
        unsigned int sum = 0;
        while (tmp != (seq_t)0) {
            seq_t digit = tmp % (seq_t)8;
            sum += (unsigned int)digit;
            tmp /= (seq_t)8;
        }

        if (sum != 0 && (candidate % (seq_t)sum == (seq_t)0)) {
            ensure_cache_capacity(cache_len + 1);
            seq_cache[cache_len++] = candidate;
        }
        ++candidate;
    }
    return seq_cache[n - 1];
}
