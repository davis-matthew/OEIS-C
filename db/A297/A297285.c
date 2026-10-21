/*
 * A297285  Numbers whose base-15 digits have greater down-variation than
 *          up-variation; see Comments.
 * https://oeis.org/A297285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th integer whose base‑15 most significant digit is
 *            greater than its least significant digit. This is equivalent to
 *            the OEIS condition DV(n,15) > UV(n,15)
 * Range:     u8 n <= 108, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* array holding the found terms          */
static size_t cache_size = 0; /* number of terms currently stored      */
static size_t cache_cap   = 0; /* current capacity of the cache         */
static seq_t next_to_test = 15;/* next integer to examine              */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n==0, but harmless      */
    /* Build the cache until it contains n terms. */
    while (cache_size < n) {
        seq_t val = next_to_test;

        /* Determine least–significant and most–significant
           base‑15 digits of val. */
        seq_t lsd = val % 15;
        seq_t msd = val;
        while (msd >= 15) msd /= 15;

        /* Store the value if msd > lsd. */
        if (msd > lsd) {
            if (cache_cap == 0) {
                cache_cap = 16;
                cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
            }
            if (cache_size >= cache_cap) {
                cache_cap *= 2;
                cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_size++] = val;
        }
        ++next_to_test;
    }
    return cache[n - 1];
}
