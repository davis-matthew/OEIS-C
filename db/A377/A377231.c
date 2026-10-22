/*
 * A377231  a(n) = digital root of 2^Fibonacci(n).
 * https://oeis.org/A377231
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = (a(n-1) * a(n-2) - 1) mod 9 + 1 with a(0) = 1, a(1) = 2
 *            (period 24)
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Compute the sequence A377231: digital root of 2^Fibonacci(n).  */
seq_t a(idx_t n)
{
    /* 24‑term period (computed once) */
    static seq_t period[24];
    static bool initialized = false;

    if (!initialized) {
        period[0] = (seq_t)1;
        period[1] = (seq_t)2;
        for (idx_t i = 2; i < 24; ++i) {
            seq_t t = period[i - 1] * period[i - 2] - 1;
            t %= (seq_t)9;
            t += (seq_t)1;
            period[i] = t;
        }
        initialized = true;
    }

    return period[n % 24];
}
