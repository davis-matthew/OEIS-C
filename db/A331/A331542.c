/*
 * A331542  The speed of light in furlongs per fortnight.
 * https://oeis.org/A331542
 *
 * Offset:    13    (a(13) is the first term)
 * Keywords:  nonn, cons, less, dumb
 * Formula:   a(n) = decimal digits of 2518256647200000/1397, offset 13
 * Range:     u8 none, u16 none, u32 none, u64 n <= 104, u128 n <= 104,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 13) return (seq_t)0;

    /* constants for the fraction NUM/DEN */
    static const idx_t NUM = 2518256647200000ULL;
    static const idx_t DEN = 1397ULL;

    /* static cache state */
    static seq_t cache[20000];   /* enough for n up to ~10000 */
    static idx_t cache_len = 0;  /* number of digits already cached */
    static seq_t frac_rem = 0;   /* remainder after the last cached digit */
    static bool initialized = false;

    /* perform one‑time initialization: compute integer digits */
    if (!initialized) {
        seq_t q = (seq_t)NUM / (seq_t)DEN;   /* integer part */
        seq_t r = (seq_t)NUM % (seq_t)DEN;   /* start remainder for fraction */

        /* store the 13 integer digits from most‑significant to least‑significant */
        for (idx_t i = 13; i-- > 0;) {
            cache[i] = q % (seq_t)10;
            q /= (seq_t)10;
        }
        cache_len = 13;    /* 13 integer digits cached */
        frac_rem = r;      /* remainder ready to generate fractional digits */
        initialized = true;
    }

    /* position of the requested digit relative to the first cached digit */
    idx_t pos = n - 13;

    /* extend the cache if the requested index is beyond what we have */
    if (pos >= cache_len) {
        for (idx_t i = cache_len; i <= pos; ++i) {
            seq_t digit = (frac_rem * (seq_t)10) / (seq_t)DEN;
            frac_rem      = (frac_rem * (seq_t)10) % (seq_t)DEN;
            cache[i]      = digit;
        }
        cache_len = pos + 1;
    }

    return cache[pos];
}
