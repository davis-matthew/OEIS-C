/*
 * A261231  a(n) = number of steps to reach 0 when starting from k = n and
 *          repeatedly applying the map that replaces k with k - (sum of
 *          digits in base-3 representation of k).
 * https://oeis.org/A261231
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + a(n - sumDigitsBase3(n)), a(0) = 0 where
 *            sumDigitsBase3(n) = sum of the base‑3 digits of n
 * Range:     u8 n <= 1646, u16 n <= 9935, u32 n <= 9935, u64 n <= 9935,
 *            u128 n <= 9935, big any n
 * Helpers:   _sum_digits_base3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_sum_digits_base3.h"

seq_t a(idx_t n)
{
    /*  Count the number of times the map k → k - sumDigitsBase3(k)
        must be applied until the value reaches 0. */
    seq_t steps = 0;
    while (n != 0) {
        idx_t sub = _sum_digits_base3(n);
        n -= sub;
        steps++;
    }
    return steps;
}
