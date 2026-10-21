/*
 * A270803  Formal inverse of Thue-Morse sequence A010060.
 * https://oeis.org/A270803
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9079, u16 n <= 9079, u32 n <= 9079, u64 n <= 9079,
 *            u128 n <= 9079, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(0)=0, a(1)=a(2)=1, a(3)=0; for n≥4
 *   if n≡0 (mod 4) then a(n)=a(n-1);
 *   if n≡1 (mod 4) then a(n)=a(n-2);
 *   if n≡2 (mod 4) then a(n)=a(n-3);
 *   if n≡3 (mod 4) then a(n)=(a(n-4)+a((n‑3)/4)) mod 2.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache the computed values for O(1) future access */
    static seq_t *cache = NULL;    /* array of a(i) for 0≤i<cache_len */
    static size_t cache_len = 0;   /* number of cached elements */

    if (n >= cache_len) {
        size_t old_len = cache_len;
        size_t new_len = (size_t)n + 1;          /* n is idx_t (uint64_t) */
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));

        /* Ensure the base values are correctly set for the new capacity */
        if (new_len > 0 && old_len < 1) cache[0] = 0;
        if (new_len > 1 && old_len < 2) cache[1] = 1;
        if (new_len > 2 && old_len < 3) cache[2] = 1;
        if (new_len > 3 && old_len < 4) cache[3] = 0;

        cache_len = new_len;

        /* Compute the new terms (starting from max(old_len,4)) */
        idx_t start = old_len < 4 ? 4 : old_len;
        for (idx_t i = start; i <= n; ++i) {
            switch ((unsigned)(i & 3u)) {
                case 0:
                    cache[i] = cache[i-1];
                    break;
                case 1:
                    cache[i] = cache[i-2];
                    break;
                case 2:
                    cache[i] = cache[i-3];
                    break;
                case 3:
                    cache[i] = (cache[i-4] + cache[(i-3)/4]) & (seq_t)1;
                    break;
            }
        }
    }

    return cache[n];
}
