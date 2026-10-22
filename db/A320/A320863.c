/*
 * A320863  Powers of 2 with initial digit 7.
 * https://oeis.org/A320863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 2, u128 n <= 6, big any n
 * Helpers:   append_candidate, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Powers of 2 with initial digit 7.
   The function iteratively generates powers of two, using the fractional part
   of k·log10(2) to test whether the leading decimal digit is 7.  Matching
   powers are stored in a static cache to allow repeated calls to any n. */

#include <stddef.h>
#include "../utils/append_candidate.h"
#include "../utils/seq_mul.h"

#define LOG10_2 0.3010299956639811952137388947245L
#define LOG10_7 0.845098040014256831138828628487L
#define LOG10_8 0.903089986991943484590348292487L

seq_t a(idx_t n)
{
    /* Static cache of found values */
    static seq_t *cache          = NULL;
    static idx_t cache_len       = 0;   /* number of cached terms          */
    static idx_t cache_cap       = 0;   /* current allocation size        */

    /* Static state for incremental search */
    static idx_t cur_k            = 0;   /* current exponent k              */
    static seq_t cur_val          = 1;   /* 2^cur_k (initialized to 2^0) */

    /* Generate powers of two until we have at least n terms */
    while (cache_len < n) {
        /* Advance to next power of two */
        ++cur_k;
        cur_val = seq_mul(cur_val, (seq_t)2);

        /* Compute fractional part of k·log10(2) */
        long double prod = (long double)cur_k * LOG10_2;
        long double frac = prod - (long double)((long long)prod);

        /* Check whether the leading digit is 7 */
        if (frac >= LOG10_7 && frac < LOG10_8) {
            /* Store the found power in the cache */
            append_candidate(&cache, &cache_len, &cache_cap, cur_val);
        }
    }

    return cache[n - 1];
}
