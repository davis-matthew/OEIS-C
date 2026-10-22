/*
 * A382286  a(n) is the least k such that floor(sqrt(n*k/d(n*k))) =
 *          floor(sqrt(d(n*k))), where d(k) is the largest divisor of k which
 *          is <= sqrt(k).
 * https://oeis.org/A382286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k ≥ 1 such that floor(sqrt(n·k / d(n·k))) =
 *            floor(sqrt(d(n·k))), where d(m) is the largest divisor of m
 *            which is ≤ sqrt(m). Computed by brute‑force search for k, with
 *            d(m) found by a downward divisor scan. Suitable for n ≤ 10 000
 *            and works under 2 s for any n in that range
 * Range:     u8 n <= 10, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined for n=0, return 0 for safety */
    seq_t N = (seq_t)n;
    seq_t k = 1;
    for (;;) {
        seq_t m = N * k;                      /* m = n·k                           */
        seq_t s = floor_sqrt(m);              /* floor(sqrt(m))                    */
        seq_t d = 0;                          /* largest divisor ≤ sqrt(m)         */
        for (seq_t i = s; i >= 1; --i) {
            if (m % i == 0) { d = i; break; } /* find first divisor from below     */
        }
        seq_t left  = floor_sqrt(d);          /* floor(sqrt(d))                    */
        seq_t right = floor_sqrt(m / d);      /* floor(sqrt(m/d))                  */
        if (left == right) return k;          /* condition satisfied               */
        ++k;
    }
}
