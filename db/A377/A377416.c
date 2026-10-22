/*
 * A377416  Least integer k which, by a process analogous to the Keith
 *          numbers, reach k + n.
 * https://oeis.org/A377416
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 290, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Search: a(n) = least integer k ≥ 10 such that the additive sequence
 * (summing the last m terms, where m is the number of decimal digits of k)
 * eventually reaches the value k + n.
 * The function implements this definition directly.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The maximal number of decimal digits in a 64‑bit unsigned integer */
    const int MAX_DIGITS = 20;
    /* Reusable buffer for the rolling sum */
    seq_t last[MAX_DIGITS];

    /* Scan candidate k from 10 upwards until one satisfies the condition */
    for (idx_t k = 10; ; ++k)
    {
        /* ----- Extract decimal digits of k (most significant first) ----- */
        idx_t tmp = k;
        int m = 0;
        int rev[MAX_DIGITS];
        while (tmp > 0) {
            rev[m++] = tmp % 10;
            tmp /= 10;
        }
        /* If k == 0 (not possible for k ≥ 10) we would continue. */

        for (int i = 0; i < m; ++i)
            last[i] = (seq_t)rev[m - 1 - i];

        /* Target value to reach: k + n */
        seq_t target = (seq_t)k + (seq_t)n;

        /* ---- Compute the first sum (after the initial digits) ---- */
        seq_t sum = 0;
        for (int i = 0; i < m; ++i)
            sum += last[i];

        if (sum == target)
            return (seq_t)k;

        /* ---- Generate the sequence until sum >= target ---- */
        while (sum < target) {
            /* Shift left: drop the oldest term */
            for (int i = 0; i + 1 < m; ++i)
                last[i] = last[i + 1];

            /* Append the new sum */
            last[m - 1] = sum;

            /* Compute the next sum */
            sum = 0;
            for (int i = 0; i < m; ++i)
                sum += last[i];

            if (sum == target)
                return (seq_t)k;
        }
    }
}
