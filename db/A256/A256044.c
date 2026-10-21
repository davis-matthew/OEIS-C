/*
 * A256044  6th row of array in A099390.
 * https://oeis.org/A256044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) follows the linear recurrence a(n) = 40*a(n‑1) – 416*a(n‑2)
 *            + 1224*a(n‑3) – 1224*a(n‑4) + 416*a(n‑5) – 40*a(n‑6) + a(n‑7)
 *            with initial values a(0)=1, a(1)=13, a(2)=281, a(3)=6728,
 *            a(4)=167089, a(5)=4213133, a(6)=106912793
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static const seq_t init[7] = { (seq_t)1, (seq_t)13, (seq_t)281,
                               (seq_t)6728, (seq_t)167089,
                               (seq_t)4213133, (seq_t)106912793 };

static seq_t *cache     = NULL;
static size_t cache_cap = 0;
static size_t cache_last = 0;   /* last index computed (>=6 after init) */

seq_t a(idx_t n)
{
    /* Return initial values directly */
    if (n < 7) return init[n];

    /* Allocate cache if first call */
    if (cache == NULL) {
        cache_cap = 64;
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (size_t i = 0; i < 7; ++i) cache[i] = init[i];
        cache_last = 6;
    }

    /* Grow cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n + 1 > new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failure – keep old cache */
            /* In a contest setting we could exit but here we simply return 0. */
            return (seq_t)0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms up to n if not already done */
    if ((size_t)n > cache_last) {
        for (size_t i = cache_last + 1; i <= (size_t)n; ++i) {
            /* Use an intermediate 128‑bit value when seq_t is a built‑in type.
               For arbitrary‑precision seq_t the built‑in operators are safe. */
            if (sizeof(seq_t) <= 8) {
                unsigned __int128 t = 0;
                t += (unsigned __int128)40   * (unsigned __int128)cache[i - 1];
                t -= (unsigned __int128)416  * (unsigned __int128)cache[i - 2];
                t += (unsigned __int128)1224 * (unsigned __int128)cache[i - 3];
                t -= (unsigned __int128)1224 * (unsigned __int128)cache[i - 4];
                t += (unsigned __int128)416  * (unsigned __int128)cache[i - 5];
                t -= (unsigned __int128)40   * (unsigned __int128)cache[i - 6];
                t += (unsigned __int128)      cache[i - 7];
                cache[i] = (seq_t)t;
            } else {
                cache[i] = (seq_t)40 * cache[i - 1]
                         - (seq_t)416 * cache[i - 2]
                         + (seq_t)1224 * cache[i - 3]
                         - (seq_t)1224 * cache[i - 4]
                         + (seq_t)416 * cache[i - 5]
                         - (seq_t)40 * cache[i - 6]
                         + cache[i - 7];
            }
        }
        cache_last = (size_t)n;
    }

    return cache[n];
}
