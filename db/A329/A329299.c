/*
 * A329299  Numbers whose digits are in nondecreasing order in bases 9 and 10.
 * https://oeis.org/A329299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth number whose decimal and base‑9 digits are
   nondecreasing.  The function builds the sequence on demand by
   scanning successive integers and testing the monotonicity
   condition in both bases. */

#include <stdbool.h>
#include <stdlib.h>

/* Test whether the digits of x in the given base are nondecreasing
   when read in the usual order (most significant to least
   significant).  x may be 0, in which case the result is true. */
static bool nondecreasing_in_base(seq_t x, unsigned base)
{
    if (x == (seq_t)0) return true;

    int digits[70];          /* enough for 128‑bit numbers in base 9 */
    int len = 0;
    seq_t temp = x;

    while (temp > (seq_t)0) {
        digits[len++] = (int)(temp % base);
        temp /= base;
    }

    /* digits[len-1] is the most significant digit; check
       nondecreasing: d[i] <= d[i+1] for i from MSD to LSD. */
    for (int i = len - 1; i > 0; --i) {
        if (digits[i] > digits[i - 1])
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Static cache of computed terms. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_len = 0;
    static seq_t next_candidate = 0;

    /* Initialize cache on first call. */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
        next_candidate = 0;
    }

    /* Generate terms until we have at least n terms. */
    while (cache_len < n) {
        /* Check current candidate for monotonicity in base 10 and 9. */
        if (nondecreasing_in_base(next_candidate, 10) &&
            nondecreasing_in_base(next_candidate, 9)) {
            /* Resize cache if necessary. */
            if ((size_t)cache_len >= cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = next_candidate;
        }
        next_candidate += (seq_t)1;
    }

    return cache[n - 1];
}
