/*
 * A385464  Consecutive internal states of the linear congruential
 *          pseudo-random number generator (9301*s + 49297) mod 233280 when
 *          started at 1.
 * https://oeis.org/A385464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A385464: consecutive internal states of the LCG
   (9301*s + 49297) mod 233280, starting from s=1.
   Recurrence: a(1)=1; a(n)= (9301*a(n-1)+49297) mod 233280.
   The sequence has period 233280.  */
#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;          /* cache of one period */
static const idx_t period = 233280ULL;

/* Compute a(n) for n >= 1.  Cache the first period on first use.  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined, but harmless */

    /* Initialise cache on first call */
    if (!cache) {
        cache = (seq_t *)malloc(period * sizeof(seq_t));
        if (!cache) abort();         /* allocation failure */
        cache[0] = (seq_t)1;
        for (size_t i = 1; i < period; ++i) {
            cache[i] = ((seq_t)9301 * cache[i-1] + 49297) % (seq_t)233280;
        }
    }

    idx_t idx = (n - 1) % period;    /* a(1) is cache[0] */
    return cache[idx];
}
