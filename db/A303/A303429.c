/*
 * A303429  Number of ordered pairs (k, m) of nonnegative integers such that n
 *          - 3^k - 5^m can be written as the sum of two squares.
 * https://oeis.org/A303429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: Count the number of ordered pairs (k,m) of nonnegative integers
   such that n - 3^k - 5^m is representable as a sum of two squares. */

#include <stdbool.h>

/* Check whether r can be written as x^2 + y^2 for integers x,y. */
static bool is_sum_two_squares(idx_t r)
{
    if (r == 0) return true;          /* 0 = 0^2 + 0^2 */
    idx_t temp = r;

    for (idx_t p = 2; (seq_t)p * (seq_t)p <= temp; ++p) {
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            if (p % 4 == 3 && (exp & 1)) {
                return false;   /* odd exponent of a prime ≡ 3 (mod 4) */
            }
        }
    }
    if (temp > 1) {            /* remaining prime factor */
        if (temp % 4 == 3) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    seq_t count = 0;

    for (idx_t p3 = 1; p3 <= n; ) {          /* 3^k */
        for (idx_t p5 = 1; p5 <= n; ) {      /* 5^m */
            idx_t sum = p3 + p5;
            if (sum > n) break;              /* further p5 will only increase sum */
            idx_t r = n - sum;
            if (is_sum_two_squares(r)) {
                count++;
            }
            if (p5 > n / 5) break;           /* next power would exceed n */
            p5 *= 5;
        }
        if (p3 > n / 3) break;               /* next power would exceed n */
        p3 *= 3;
    }

    return count;
}
