/*
 * A384610  Consecutive states of the linear congruential pseudo-random number
 *          generator (262145*s+3) mod 2^35 when started at s=1.
 * https://oeis.org/A384610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (262145 * a(n-1) + 3) mod 2^35, a(1) = 1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 105, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Compute the nth term of the sequence.  The function is thread‑local by virtue of
 * static storage, but it is safe for repeated calls in any order. */
seq_t a(idx_t n)
{
    /* Mask for modulo 2^35 */
    static const seq_t MASK = (((seq_t)1 << 35) - 1);

    /* Cache of previously computed terms */
    static seq_t *cache   = NULL; /* array a[0] = a(1) */
    static idx_t   cap    = 0;    /* allocated capacity (in terms) */
    static idx_t   size   = 0;    /* number of terms currently stored */

    /* Ensure we have storage for at least 'needed' terms */
    if (size < n) {
        idx_t needed = n;
        if (cap < needed) {
            idx_t new_cap = cap ? cap << 1 : 4;
            while (new_cap < needed) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);           /* allocation failure */
            cache = tmp;                  /* may be NULL if new_cap is 0 */
            cap   = new_cap;
        }
        /* Initialise if first call */
        if (size == 0) {
            cache[0] = 1;   /* a(1)=1 */
            size = 1;
        }
        /* Generate missing terms */
        for (idx_t i = size; i < n; ++i) {
            cache[i] = ((seq_t)262145 * cache[i-1] + 3) & MASK;
        }
        size = n;
    }
    return cache[n-1];
}
