/*
 * A292402  p-INVERT of (1,0,0,1,0,0,0,0,0,0,...), where p(S) = 1 - S^2.
 * https://oeis.org/A292402
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + 2*a(n-5) + a(n-8) for n>=9; Initial values:
 *            a(0)=0, a(1)=1, a(2)=0, a(3)=1, a(4)=2, a(5)=1, a(6)=4, a(7)=2,
 *            a(8)=6
 * Range:     u8 n <= 20, u16 n <= 37, u32 n <= 71, u64 n <= 140,
 *            u128 n <= 278, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *CACHE = NULL;
static size_t CACHE_CAP = 0;

/* Ensure CACHE has space for indices 0..n inclusive and all values are computed. */
static void ensure_cache(idx_t n)
{
    if ((size_t)n + 1 <= CACHE_CAP) return;
    size_t new_cap = (size_t)n + 1;
    CACHE = (seq_t *)realloc(CACHE, new_cap * sizeof(seq_t));
    if (!CACHE) abort();   /* allocation failure */
    for (size_t i = CACHE_CAP; i < new_cap; ++i) {
        switch (i) {
            case 0: CACHE[0] = 0; break;
            case 1: CACHE[1] = 1; break;
            case 2: CACHE[2] = 0; break;
            case 3: CACHE[3] = 1; break;
            case 4: CACHE[4] = 2; break;
            case 5: CACHE[5] = 1; break;
            case 6: CACHE[6] = 4; break;
            case 7: CACHE[7] = 2; break;
            case 8: CACHE[8] = 6; break;
            default:
                /* a(n) = a(n-2) + 2*a(n-5) + a(n-8) for n >= 9 */
                CACHE[i] = CACHE[i-2] + (CACHE[i-5] << 1) + CACHE[i-8];
        }
    }
    CACHE_CAP = new_cap;
}

seq_t a(idx_t n)
{
    /* All indices >= 0 are valid; the sequence is defined for n >= 0. */
    ensure_cache(n);
    return CACHE[(size_t)n];
}
