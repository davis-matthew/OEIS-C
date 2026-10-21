/*
 * A297396  Number of 3 X n 0..1 arrays with every 1 horizontally, diagonally
 *          or antidiagonally adjacent to 1 neighboring 1.
 * https://oeis.org/A297396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + a(n-2) + 8*a(n-3) + a(n-4) - 2*a(n-6);
   Initial values: a(1)=1, a(2)=9, a(3)=19, a(4)=37, a(5)=127, a(6)=323 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle indices below the offset (undefined) */
    if (n == 0) return (seq_t)0;

    /* Base cases for n <= 6 */
    switch (n) {
        case 1: return (seq_t)1;
        case 2: return (seq_t)9;
        case 3: return (seq_t)19;
        case 4: return (seq_t)37;
        case 5: return (seq_t)127;
        case 6: return (seq_t)323;
        default: break;
    }

    /* Use a sliding window of the last six terms to compute iteratively.
       This uses O(1) memory and works for any idx_t. */
    seq_t t[7];                   /* t[1]..t[6] hold a(n-6)..a(n-1) */
    t[1] = (seq_t)1;
    t[2] = (seq_t)9;
    t[3] = (seq_t)19;
    t[4] = (seq_t)37;
    t[5] = (seq_t)127;
    t[6] = (seq_t)323;
    seq_t v;

    for (idx_t i = 7; i <= n; ++i) {
        /* recurrence: a(n) = a(n‑1) + a(n‑2) + 8*a(n‑3) + a(n‑4) – 2*a(n‑6) */
        v = t[6] + t[5] + (seq_t)8 * t[4] + t[3] - (seq_t)2 * t[1];

        /* shift window: drop t[1], move others left, insert new value */
        t[1] = t[2];
        t[2] = t[3];
        t[3] = t[4];
        t[4] = t[5];
        t[5] = t[6];
        t[6] = v;
    }

    return t[6];
}
