/*
 * A320071  Number of length n primitive (=aperiodic or period n) 6-ary words
 *          which are earlier in lexicographic order than any other word
 *          derived by cyclic shifts of the alphabet.
 * https://oeis.org/A320071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} 6^(d-1) * μ(n/d)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   mul_small, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence can be computed incrementally by the recurrence
   a(1) = 1,
   a(n) = 6^(n-1) - Σ_{d|n, d<n} a(d)
   for n ≥ 2.  The implementation keeps a cache of computed
   values and a cache of powers of 6. */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* ----------   static state   ---------- */
    static seq_t *a_cache = NULL;   /* a_cache[i] = a(i) for 0 ≤ i ≤ maxn */
    static seq_t *pow_cache = NULL; /* pow_cache[i] = 6^i for 0 ≤ i ≤ maxpow */
    static size_t a_size = 0;       /* allocated length of a_cache */
    static size_t pow_size = 0;     /* allocated length of pow_cache */
    static idx_t maxn = 0;          /* largest n for which a_cache has been filled */
    static idx_t maxpow = 0;        /* largest exponent for which pow_cache holds 6^e */

    if (n < 1) return 0;                /* undefined below offset */

    /* ----------   allocate a_cache up to n   ---------- */
    if ((size_t)n + 1 > a_size) {
        size_t new_size = (size_t)n + 1;
        if (a_cache == NULL) {
            a_cache = seq_alloc(new_size);
            a_size  = new_size;
            a_cache[0] = 0;         /* unused, but convenient */
        } else {
            a_cache = seq_realloc(a_cache, a_size, new_size);
            a_size  = new_size;
        }
    }

    /* ----------   allocate pow_cache up to n-1   ---------- */
    if (n > 0 && (size_t)n > pow_size) { /* need 6^(n-1) so upto index n-1 */
        size_t new_size = (size_t)n;      /* indices 0..n-1 inclusive */
        if (pow_cache == NULL) {
            pow_cache = seq_alloc(new_size);
            pow_size  = new_size;
            pow_cache[0] = 1;          /* 6^0 = 1 */
            maxpow = 0;
        } else {
            pow_cache = seq_realloc(pow_cache, pow_size, new_size);
            pow_size  = new_size;
        }
        /* compute any missing powers */
        for (size_t i = maxpow + 1; i < new_size; ++i) {
            pow_cache[i] = mul_small(pow_cache[i-1], 6);
        }
        maxpow = (size_t)(new_size - 1); /* last computed exponent */
    }

    /* ----------   compute missing a values   ---------- */
    if (maxn < 1) {          /* set base case a(1) = 1 */
        a_cache[1] = 1;
        maxn = 1;
    }

    for (idx_t i = maxn + 1; i <= n; ++i) {
        seq_t total = pow_cache[(size_t)(i - 1)];   /* 6^(i-1) */

        /* subtract a(d) for every proper divisor d < i */
        for (size_t d = 1; d * d <= (size_t)i; ++d) {
            if ((size_t)i % d == 0) {
                if (d < (size_t)i) total -= a_cache[d];
                size_t d2 = (size_t)i / d;
                if (d2 != d && d2 < (size_t)i) {
                    total -= a_cache[d2];
                }
            }
        }
        a_cache[i] = total;
    }
    maxn = n;

    return a_cache[n];
}
