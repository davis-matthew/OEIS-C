/*
 * A265688  Binary representation of the n-th iteration of the "Rule 190"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A265688
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 100*a(n-1) + a(n-2) - 100*a(n-3); a(0) = 1, a(1) = 111,
 *            a(2) = 11101
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Cache a fixed number of terms so that repeated calls are cheap.
       20001 is more than enough for the required n ≤ 10000, and it
       also works for the arbitrary‑precision build (the array contains
       fully constructed objects in C++). */
    enum { CACHE_MAX = 20001 };
    static seq_t cache[CACHE_MAX];
    static idx_t cache_last = 2;          /* highest index currently cached */
    static bool init = false;              /* one‑time initialisation */

    if (!init) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)111;
        cache[2] = (seq_t)11101;
        init = true;
    }

    /* Return cached value if already computed */
    if (n <= cache_last)
        return cache[n];

    /* Use the three most recent cached values as the starting point */
    seq_t a0 = cache[cache_last - 2];
    seq_t a1 = cache[cache_last - 1];
    seq_t a2 = cache[cache_last];

    /* Compute terms up to n using the recurrence */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        seq_t a3 = (seq_t)100 * a2 + a1 - (seq_t)100 * a0;

        /* Store the value while we are still inside the cache range */
        if (i < CACHE_MAX)
            cache[i] = a3;

        /* Shift the window for the next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
    }

    /* Update cache_last if we stored terms in the array */
    if (n < CACHE_MAX)
        cache_last = n;

    /* a2 now holds a(n) */
    return a2;
}
