/*
 * A321417  Number of n element multisets of the 12th roots of unity with zero
 *          sum.
 * https://oeis.org/A321417
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of n‑element multisets of the 12th roots of unity
 *            with zero sum
 * Range:     u8 n <= 9, u16 n <= 27, u32 n <= 157, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from G.f.: ((2/(1-x^3)-1)/(1-x^2)^3)^2   */
#include <stdlib.h>
#include <string.h>

/* Rational form G(x) = P(x)/Q(x) with   P(x) = (1-x+x^2)^2,
   Q(x) = (1-x)^8 (1+x)^4 (1+x+x^2)^2.
   The denominator expands to a palindrome of degree 16:
   Q(x) = 1 -2x-3x^2+6x^3+6x^4-6x^5-13x^6+2x^7+18x^8+2x^9-13x^10-6x^11+6x^12+6x^13-3x^14-2x^15+x^16.  */
static const int16_t Q[17] = {
    1,   /* Q[0]  (not used in recurrence) */
   -2,  -3,   6,   6,  -6, -13,   2,  18,   2,
 -13,  -6,   6,   6,  -3,  -2,   1   /* Q[16] */
};

static const seq_t BASE[5] = {
    (seq_t)1, /* a(0) */
    (seq_t)0, /* a(1) */
    (seq_t)6, /* a(2) */
    (seq_t)4, /* a(3) */
    (seq_t)21 /* a(4) */
};

static seq_t *cache = NULL;
static size_t cache_len = 0;      /* length of allocated array */
static idx_t   cache_computed = (idx_t)-1; /* maximum n for which cache[n] is known */

static void extend_cache(idx_t n)
{
    if (n < cache_len) return;                  /* already have enough space */
    size_t new_len = cache_len ? cache_len * 2 : 16;
    while (new_len <= (size_t)n) new_len <<= 1;
    seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
    if (!new_cache) { abort(); }
    if (cache) {
        memcpy(new_cache, cache, cache_len * sizeof(seq_t));
        free(cache);
    }
    cache = new_cache;
    cache_len = new_len;
}

seq_t a(idx_t n)
{
    /* Ensure sufficient storage */
    if (n >= cache_len || cache_computed < n) {
        extend_cache(n);
        if ((idx_t)cache_computed < 0) cache_computed = 0;
        for (idx_t k = cache_computed + 1; k <= n; ++k) {
            if (k < 5) {                          /* base cases */
                cache[k] = BASE[(int)k];
            } else {
                __int128 sum = 0;
                /* Sum Q[i] * a[n-i]   (Q[0]=1 is not used in the sum) */
                for (int i = 1; i <= 16; ++i) {
                    if (k < (idx_t)i) break;
                    sum += (__int128)Q[i] * (__int128)cache[k - (idx_t)i];
                }
                cache[k] = (seq_t)(-sum);         /* a(n) = -sum  (Q[0]==1) */
            }
        }
        cache_computed = n;
    }
    return cache[n];
}
