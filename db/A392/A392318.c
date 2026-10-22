/*
 * A392318  Decimal expansion of 37/72.
 * https://oeis.org/A392318
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor((10^(n+1) · 37)/72) mod 10 (digits of 37/72)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache of already computed digits */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static idx_t next_to_compute = 0;
    static unsigned long long rem = 37;          /* remainder before computing a(next) */

    /* initialise on first call */
    if (cache == NULL) {
        cache_len = 1;
        cache = (seq_t*)malloc(sizeof(seq_t));
        unsigned long long t = rem * 10ULL;        /* 37*10 */
        cache[0] = (seq_t)(t / 72ULL);             /* first digit is 5 */
        rem = t % 72ULL;                           /* new remainder 10 */
        next_to_compute = 1;
    }

    /* extend cache up to requested index */
    while (next_to_compute <= n) {
        unsigned long long t = rem * 10ULL;
        seq_t digit = (seq_t)(t / 72ULL);
        rem = t % 72ULL;

        if (next_to_compute >= cache_len) {
            cache_len = next_to_compute + 1;
            cache = (seq_t*)realloc(cache, cache_len * sizeof(seq_t));
        }
        cache[next_to_compute] = digit;
        ++next_to_compute;
    }

    return cache[n];
}
