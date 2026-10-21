/*
 * A250136  Number of length n+2 0..4 arrays with the medians of every three
 *          consecutive terms nondecreasing.
 * https://oeis.org/A250136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of arrays of length n+2 over {0,1,2,3,4} with
 *            non‑decreasing medians of every three consecutive terms
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 15, u64 n <= 36, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  We maintain the last three terms of the array.
    State (a,b,c) -> median Mabc.
    When a new term x is appended we must have
          Mabc <= Mbcr
    where M(b,c,x) is the median of the next three‑tuple.
    This is a simple DP over 5³ = 125 states.  */

#include <stddef.h>   /* for size_t */

static inline int median3(int x, int y, int z)
{
    /* small integer median */
    int t1 = (x < y) ? x : y;
    int t2 = (x < y) ? y : x;
    if (z < t1) return t1;
    if (z > t2) return t2;
    return z;
}

seq_t a(idx_t n)
{
    /* special case: n = 1 -> all 5³ = 125 arrays */
    if (n == 1) return (seq_t)125;

    const int STATES = 125;      /* 5³ */
    seq_t cur[STATES];
    seq_t nxt[STATES];
    /* initialise current states with one way for each triple */
    for (int i = 0; i < STATES; ++i) cur[i] = 1;

    /* number of transitions required = (n+2) - 3 = n-1 */
    idx_t step;
    for (step = 1; step < n; ++step) {
        /* zero next array */
        for (int i = 0; i < STATES; ++i) nxt[i] = 0;

        for (int s = 0; s < STATES; ++s) {
            if (!cur[s]) continue;

            /* decode state: a,b,c from index s = a*25 + b*5 + c */
            int a = s / 25;
            int b = (s / 5) % 5;
            int c = s % 5;

            int Mabc = median3(a, b, c);

            for (int x = 0; x < 5; ++x) {
                int Mbcx = median3(b, c, x);
                if (Mabc <= Mbcx) {
                    int ns = b * 25 + c * 5 + x;
                    nxt[ns] += cur[s];
                }
            }
        }
        /* copy nxt to cur for next iteration */
        for (int i = 0; i < STATES; ++i) cur[i] = nxt[i];
    }

    /* sum all counts for the final length */
    seq_t total = 0;
    for (int i = 0; i < STATES; ++i) total += cur[i];
    return total;
}
