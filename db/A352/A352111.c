/*
 * A352111  In the factorial base expansion of n, replace each place value,
 *          say k! with k > 0, by (-1)^(k-1) * k!.
 * https://oeis.org/A352111
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = Σ_{k≥1} d_k * (-1)^(k-1) * k!, where n = Σ_{k≥1} d_k * k!
 *            is the factorial-base expansion of n (d_k ∈ [0, k])
 * Range:     i8 n <= 131, i16 n <= 5039, i32 n <= 5039, i64 n <= 5039,
 *            i128 n <= 5039, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Pre‑compute factorials up to the largest k with k! ≤ n.
       Use __int128 for safety; results finally cast to seq_t.
       The maximum k for 64‑bit n is 20, so the array size 21 is safe. */
    __int128 fact[21];
    fact[0] = 1;           /* 0! (unused in the sum but convenient) */
    int kmax = 0;
    for (int k = 1; k <= 20; ++k) {
        fact[k] = fact[k - 1] * (__int128)k;
        if (fact[k] > (__int128)n) {
            kmax = k - 1;           /* largest factorial ≤ n */
            break;
        }
        if (k == 20) {               /* n very large, all 20! fits */
            kmax = 20;
        }
    }

    __int128 rem = n;           /* remaining value to process */
    __int128 sum = 0;           /* running total in __int128 */

    for (int k = kmax; k >= 1; --k) {
        __int128 digit = rem / fact[k];
        rem -= digit * fact[k];

        /* sign = (-1)^(k-1): positive for odd k, negative for even k */
        int sign = (k & 1) ? 1 : -1;

        sum += digit * sign * fact[k];
    }

    /* Convert to seq_t; overflow is acceptable for values that exceed seq_t. */
    return (seq_t)sum;
}
