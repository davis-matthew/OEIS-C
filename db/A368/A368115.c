/*
 * A368115  Trajectory of 17 under the '17x+1' map.
 * https://oeis.org/A368115
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Validated: partial, pass, 78 terms through n = 77 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Trajectory of 17 under the '17x+1' map
   a(0) = 17
   For n > 0:
      if a(n-1) = 1           -> a(n) = 17*a(n-1) + 1
      else if a(n-1) even     -> a(n) = a(n-1) / 2
      else if a(n-1) <= 5     -> a(n) = 1          (i.e. divide by itself)
      else if a(n-1) is prime -> a(n) = 17*a(n-1) + 1
      else (odd composite)   -> a(n) = a(n-1) / spf(a(n-1))
   (spf = the smallest prime factor)
   This implements the sequence A368115. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Return true if n is prime; on exit *spf holds the smallest prime factor of n,
   which equals n itself when n is prime. */
static bool get_spf(seq_t n, seq_t *spf)
{
    seq_t limit = floor_sqrt(n);
    for (seq_t d = (seq_t)3; d <= limit; d += (seq_t)2) {
        if ((n % d) == 0) {
            *spf = d;
            return false;           /* composite */
        }
    }
    *spf = n;
    return true;                    /* prime */
}

/* Compute the next value in the trajectory from current value x. */
static seq_t next_value(seq_t x)
{
    if (x == (seq_t)1) {
        return ((seq_t)17 * x) + (seq_t)1;      /* 18 */
    }
    if ((x & (seq_t)1u) == (seq_t)0u) {        /* even */
        return x / (seq_t)2;
    }
    /* odd and >1 */
    if (x <= (seq_t)5) {                        /* 3 or 5 */
        return x / x;                           /* 1 */
    }
    seq_t spf;
    bool prime = get_spf(x, &spf);
    if (prime) {                                 /* prime >5 */
        return ((seq_t)17 * x) + (seq_t)1;
    } else {                                     /* odd composite */
        return x / spf;
    }
}

/* Cache data for memoization */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_len = 0;

/* Return the nth term of the sequence.  cache_len is always >= 1 after
   the first call.  The function works for all n >= 0. */
seq_t a(idx_t n)
{
    if (cache == NULL) {
        cache_cap = 16;
        cache = mem_new(seq_t, cache_cap);
        cache[0] = (seq_t)17;
        cache_len = 1;
    }

    if (n >= cache_len) {
        size_t needed = (size_t)n + 1;
        if (needed > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            while (new_cap < needed) new_cap *= 2;
            cache = mem_resize(seq_t, cache, new_cap);
            cache_cap = new_cap;
        }
        for (idx_t i = cache_len; i <= n; ++i) {
            cache[i] = next_value(cache[i - 1]);
        }
        cache_len = n + 1;
    }

    return cache[n];
}
