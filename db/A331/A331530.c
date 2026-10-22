/*
 * A331530  a(n) is the number of nonnegative integers that can be represented
 *          in a 7-segment display by using only n segments (version A010371).
 * https://oeis.org/A331530
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = a(n-2) + 2*a(n-4) + 3*a(n-5) + 3*a(n-6) + a(n-7) for n >
 *            13 initial values a(0..13) are hard‑coded as shown in the OEIS
 *            entry
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 45, u64 n <= 88, u128 n <= 174,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init[14] = {
    0ULL, 0ULL, 1ULL, 0ULL, 3ULL, 3ULL,
    8ULL, 7ULL, 16ULL, 23ULL, 48ULL, 70ULL,
    125ULL, 192ULL
};

seq_t a(idx_t n)
{
    /* A small optimization: if n is within the known prefix, just return. */
    if (n < 14)
        return init[(size_t)n];

    /* Allocate one entry per index up to n. */
    size_t len = (size_t)n + 1;
    seq_t *t = (seq_t *)calloc(len, sizeof(seq_t));
    if (!t) return 0; /* allocation failure – unlikely */

    /* Copy the known initial values. */
    for (size_t i = 0; i < 14; ++i)
        t[i] = init[i];

    /* Compute the sequence up to n using the recurrence. */
    for (size_t i = 14; i <= (size_t)n; ++i) {
        t[i] = t[i-2]
             + (seq_t)2 * t[i-4]
             + (seq_t)3 * t[i-5]
             + (seq_t)3 * t[i-6]
             + t[i-7];
    }

    seq_t res = t[(size_t)n];
    free(t);
    return res;
}
