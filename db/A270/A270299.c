/*
 * A270299  Numbers which are representable as a sum of eleven but no fewer
 *          consecutive nonnegative integers.
 * https://oeis.org/A270299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 340, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * This function returns the n-th positive integer that can be written as
 * the sum of 11 consecutive nonnegative integers and cannot be written as
 * the sum of k consecutive nonnegative integers for any 2 ≤ k ≤ 10.
 *
 * It enumerates the starting value m of the 11-term block, tests all
 * shorter lengths, and returns the N = 11·m + 55 that satisfies the
 * condition.
 */
#include <stdbool.h>

seq_t a(idx_t n) {
    idx_t count = 0;
    idx_t m     = 0;                /* starting integer of the 11-term block */

    while (count < n) {
        seq_t N = (seq_t)11 * m + (seq_t)55;   /* sum of 11 consecutive terms */

        bool ok = true;
        for (idx_t k = 2; k <= 10; ++k) {
            seq_t kk  = (seq_t)k;
            seq_t sub = kk * (kk - 1) / 2;   /* k(k-1)/2 */

            if (sub > N) break;              /* larger k will only increase sub */

            seq_t rem = N - sub;
            if (rem % kk == 0) {             /* a shorter representation exists */
                ok = false;
                break;
            }
        }

        if (ok) {
            ++count;
            if (count == n) return N;
        }
        ++m;
    }

    /* The function should never reach here for n ≥ 1 */
    return 0;
}
