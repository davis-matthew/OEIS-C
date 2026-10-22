/*
 * A351472  Numbers m such that the largest digit in the decimal expansion of
 *          1/m is 6.
 * https://oeis.org/A351472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

/* Computes term via search: checks each integer for the property that
   1/m's decimal expansion has largest digit 6. */
seq_t a(idx_t n)
{
    /* Static state for memoization */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t last_checked = (seq_t)1;

    /* Very small indices are safe to return 0 (undefined). */
    if (n == (idx_t)0) return (seq_t)0;

    /* Find the n‑th term if it hasn't been cached yet. */
    while (cache_len < (size_t)n) {
        /* Scan forward until the next term is found. */
        for (seq_t cand = last_checked; ; ++cand) {
            /* Check whether cand satisfies the property. */
            seq_t m = cand;
            /* Strip trailing zeros – they never affect the digits of 1/m. */
            while ((m % (seq_t)10) == (seq_t)0) m /= (seq_t)10;
            if (m < (seq_t)6) continue;              /* smallest term is 6 */

            bool had6 = false;
            seq_t maxd = 0;
            seq_t r = (seq_t)1;
            seq_t limit = m;                         /* at most m steps needed */

            for (seq_t i = (seq_t)0; i < limit; ++i) {
                seq_t tmp = r * (seq_t)10;
                seq_t digit = tmp / m;
                r = tmp % m;

                if (digit > (seq_t)6) {              /* digit too large */
                    maxd = (seq_t)7;                 /* force rejection */
                    break;
                }
                if (digit == (seq_t)6) had6 = true;
                if (digit > maxd) maxd = digit;

                if (r == (seq_t)0) break;            /* terminating decimal */
            }

            if (had6 && maxd <= (seq_t)6) {
                /* Grow cache if necessary. */
                if (cache_len == cache_cap) {
                    cache_cap = cache_cap ? cache_cap * 2 : 8;
                    seq_t *tmp = (seq_t *)realloc(cache,
                                                   cache_cap * sizeof(seq_t));
                    if (!tmp) abort();               /* allocation failure */
                    cache = tmp;
                }
                cache[cache_len++] = cand;
                last_checked = cand + (seq_t)1;
                break;                               /* found a term */
            }
        }
    }
    return cache[n - 1];
}
