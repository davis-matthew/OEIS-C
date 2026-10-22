/*
 * A300421  Number of n X 2 0..1 arrays with every element equal to 1, 2 or 3
 *          horizontally, vertically or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A300421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence a(1) = 1, a(2) = 3, a(3) = 10, a(4) = 30,
 *            a(5) = 96, a(6) = 307, a(7) = 981 a(n) = 3*a(n-1) + a(n-2) -
 *            3*a(n-4) - 2*a(n-5) - a(n-6) for n > 7 (see OEIS A300421)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 39, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* static storage for memoisation */
static seq_t *cache = NULL;
static size_t cache_cap = 0;    /* allocated capacity */
static idx_t  current_max = 0;  /* highest n for which cache[0..current_max-1] are valid */

/* ------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* n must be >= 1; for n == 0 we return 0 (unused in the sequence)    */
    if (n == 0)
        return 0;

    /* Grow the cache array if necessary */
    if ((size_t)n > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
        cache = tmp;
        cache_cap = (size_t)n;
    }

    /* If we already have the value, return it immediately */
    if (current_max >= n)
        return cache[(size_t)(n - 1)];

    /* Initialise the first seven terms if this is the first call or
       the cache has been cleared (current_max == 0).  We initialise
       as many as are needed (up to n).                                    */
    if (current_max == 0) {
        if (n >= 1) cache[0] = (seq_t)1;   /* a(1) */
        if (n >= 2) cache[1] = (seq_t)3;   /* a(2) */
        if (n >= 3) cache[2] = (seq_t)10;  /* a(3) */
        if (n >= 4) cache[3] = (seq_t)30;  /* a(4) */
        if (n >= 5) cache[4] = (seq_t)96;  /* a(5) */
        if (n >= 6) cache[5] = (seq_t)307; /* a(6) */
        if (n >= 7) cache[6] = (seq_t)981; /* a(7) */
        current_max = (current_max < n) ? n : current_max;
    }

    /* Compute missing terms up to n using the recurrence */
    for (idx_t i = current_max + 1; i <= n; ++i) {
        size_t idx = (size_t)(i - 1);   /* array index for a(i) */
        seq_t val;

        if (i <= 7) {
            switch ((int)i) {
                case 1: val = 1;   break;
                case 2: val = 3;   break;
                case 3: val = 10;  break;
                case 4: val = 30;  break;
                case 5: val = 96;  break;
                case 6: val = 307; break;
                case 7: val = 981; break;
                default: val = 0;  /* should never happen */
            }
        } else {
            /* a(i) = 3*a(i-1) + a(i-2) - 3*a(i-4) - 2*a(i-5) - a(i-6) */
            seq_t t1 = cache[(size_t)(i - 2)]; /* a(i-1) */
            seq_t t2 = cache[(size_t)(i - 3)]; /* a(i-2) */
            seq_t t3 = cache[(size_t)(i - 5)]; /* a(i-4) */
            seq_t t4 = cache[(size_t)(i - 6)]; /* a(i-5) */
            seq_t t5 = cache[(size_t)(i - 7)]; /* a(i-6) */

            /* temp = 3*t1 + t2 */
            seq_t temp = t1 + t1 + t1 + t2;
            /* sub = 3*t3 + 2*t4 + t5 */
            seq_t sub = t3 + t3 + t3 + t4 + t4 + t5;
            val = temp - sub; /* guaranteed non‑negative by the recurrence */
        }

        cache[idx] = val;
    }

    current_max = n;
    return cache[(size_t)(n - 1)];
}
