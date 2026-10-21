/*
 * A266980  Decimal representation of the n-th iteration of the "Rule 79"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266980
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 21*a(n-2) - 84*a(n-4) + 64*a(n-6) for n > 5, with initial
 *            values: a(0) = 1, a(1) = 6, a(2) = 5, a(3) = 122, a(4) = 21,
 *            a(5) = 2026 (For n ≤ 5 the function simply returns the table
 *            value.)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t, used in array indexing */

seq_t a(idx_t n)
{
    /* Raw initial values as 64‑bit constants – no compile‑time overflow on
       unsigned types smaller than 64 bits.  They are converted to seq_t
       (possibly truncated if seq_t is too small; that is OK for terms that
       do not fit in seq_t). */
    static const uint64_t raw_init[6] = {1, 6, 5, 122, 21, 2026};

    /* Return early for the first six terms. */
    if (n < 6)
        return (seq_t)raw_init[n];

    /* Sliding window of the previous six values. */
    seq_t a_prev[6];
    for (int i = 0; i < 6; ++i)
        a_prev[i] = (seq_t)raw_init[i];

    /* Compute terms up to n.  We use addition first to avoid
       underflow of the unsigned arithmetic. */
    for (idx_t i = 6; i <= n; ++i)
    {
        size_t idx0 = (size_t)(i - 6) % 6;   /* a(i-6) */
        size_t idx1 = (size_t)(i - 4) % 6;   /* a(i-4) */
        size_t idx2 = (size_t)(i - 2) % 6;   /* a(i-2) */

        seq_t t1 = a_prev[idx2] * (seq_t)21;   /* 21*a(i-2) */
        seq_t t3 = a_prev[idx0] * (seq_t)64;   /* 64*a(i-6) */
        seq_t t2 = a_prev[idx1] * (seq_t)84;   /* 84*a(i-4) */

        seq_t tmp = t1 + t3;          /* 21*a(i-2) + 64*a(i-6) */
        seq_t val = tmp - t2;         /* subtract 84*a(i-4) */

        a_prev[i % 6] = val;          /* store for future use */
    }

    return a_prev[n % 6];
}
