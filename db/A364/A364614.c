/*
 * A364614  Numbers not divisible by any prime of the form 3*k - 1.
 * https://oeis.org/A364614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer whose every prime divisor is
 *            NOT congruent to 2 modulo 3 (i.e. not of the form 3*k-1)
 * Range:     u8 n <= 52, u16 n <= 9076, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the first term directly. */
    if (n == 1) return (seq_t)1;

    /* The desired index. */
    size_t target = (size_t)n;

    /* Starting sieve limit: a rough upper bound proportional to n. */
    size_t limit = target * 10;
    if (limit < 200) limit = 200;      /* ensure a minimal range */

    while (1) {
        /* Smallest‑prime‑factor array for [0..limit]. */
        uint32_t *spf = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
        if (!spf) return (seq_t)0;       /* memory failure fallback */

        /* Initialize. */
        for (size_t i = 0; i <= limit; ++i) spf[i] = 0;
        spf[1] = 1;

        /* Simple sieve building spf. */
        for (size_t p = 2; p <= limit; ++p) {
            if (spf[p] == 0) {           /* p is prime */
                spf[p] = (uint32_t)p;
                if (p <= limit / p) {
                    for (size_t j = p * p; j <= limit; j += p)
                        if (spf[j] == 0) spf[j] = (uint32_t)p;
                }
            }
        }

        /* Count allowed numbers, starting with 1. */
        size_t count = 1;               /* the number 1 is always allowed */

        for (size_t v = 2; v <= limit; ++v) {
            size_t tmp = v;
            int ok = 1;
            while (tmp > 1) {
                uint32_t prime = spf[tmp];
                if (prime % 3 == 2) {    /* forbidden prime factor found */
                    ok = 0;
                    break;
                }
                tmp /= prime;
            }
            if (ok) {
                ++count;
                if (count == target) {
                    free(spf);
                    return (seq_t)v;
                }
            }
        }

        /* Not enough terms; double the limit and try again. */
        free(spf);
        if (limit > SIZE_MAX / 2) break;    /* safety */
        limit *= 2;
    }

    /* In the unlikely event of failure, return zero. */
    return (seq_t)0;
}
