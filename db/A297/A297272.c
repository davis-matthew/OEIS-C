/*
 * A297272  Numbers whose base-10 digits have greater up-variation than
 *          down-variation; see Comments.
 * https://oeis.org/A297272
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the n-th positive integer whose decimal digits have
 *            greater up-variation than down-variation
 * Range:     u8 n <= 154, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Up-variation (UV) is the sum of differences for each rising pair
   of adjacent digits; down-variation (DV) is the sum of differences
   for each falling pair.  The sequence contains numbers for which UV > DV.   */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;        /* undefined, but harmless */

    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_len  = 0;
    static seq_t cur_val     = 1;  /* next integer to test */

    while (cache_len < n)
    {
        /* compute UV and DV for cur_val */
        seq_t uv = (seq_t)0;
        seq_t dv = (seq_t)0;
        seq_t x  = cur_val;
        uint8_t dgt[512];          /* buffer for decimal digits, LSB first */
        size_t d = 0;

        /* extract decimal digits into dgt, least‑significant first */
        while (x > (seq_t)0) {
            dgt[d++] = (uint8_t)(x % (seq_t)10);
            x      /= (seq_t)10;
        }

        /* if only one digit, uv==dv==0 and the number is not selected */
        for (size_t i = 1; i < d; ++i) {
            int prev   = dgt[i];     /* more significant digit   */
            int curdig = dgt[i-1];   /* less significant digit   */
            if (curdig > prev)
                uv += (seq_t)(curdig - prev);
            else if (curdig < prev)
                dv += (seq_t)(prev - curdig);
        }

        if (uv > dv) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                    abort();          /* memory allocation failed */
                cache     = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cur_val;
        }
        ++cur_val;
    }
    return cache[n-1];
}
