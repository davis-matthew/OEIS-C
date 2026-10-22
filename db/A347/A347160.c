/*
 * A347160  Sum of 4th powers of distinct prime divisors of n that are <=
 *          sqrt(n).
 * https://oeis.org/A347160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = sum of 4th powers of distinct prime divisors of n that are ≤ sqrt(n)
 */
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2U) return 0;
    idx_t orig = n;

    /* Compute floor(sqrt(orig)) without overflow */
    idx_t lo = 0, hi = orig, sqrt_orig = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (mid == 0) { sqrt_orig = 0; break; }
        if (mid <= orig / mid) { sqrt_orig = mid; lo = mid + 1; }
        else { hi = mid - 1; }
    }

    seq_t sum = 0;
    idx_t temp = orig;

    for (idx_t i = 2; i <= sqrt_orig; ++i) {
        if (temp % i != 0) continue;

        /* check if i is prime */
        bool prime = true;
        for (idx_t j = 2; j * j <= i; ++j) {
            if (i % j == 0) { prime = false; break; }
        }
        if (!prime) continue;

        /* add i^4 to the sum */
        seq_t val = (seq_t)i;
        seq_t val2 = val * val;
        seq_t val4 = val2 * val2;
        sum += val4;

        /* remove all occurrences of i from temp */
        while (temp % i == 0) temp /= i;
    }

    return sum;
}
