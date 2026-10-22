/*
 * A359370  a(n) = 1 if n is not a multiple of 4 and has an even number of
 *          prime factors (with multiplicity), otherwise a(n) = 0.
 * https://oeis.org/A359370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is not a multiple of 4 and has an even number of
 *            prime factors (with multiplicity), otherwise 0
 * Range:     u8 n <= 9045, u16 n <= 9045, u32 n <= 9045, u64 n <= 9045,
 *            u128 n <= 9045, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Not a multiple of 4? */
    if (n % 4 == 0) return 0;

    bool even = true;               /* even number of prime factors seen so far */
    idx_t temp = n;

    while (temp > 1) {
        idx_t p = 2;
        if ((temp & 1ULL) == 0) {   /* divisible by 2 */
            p = 2;
        } else {
            /* find the smallest odd divisor */
            idx_t d = 3;
            for (; d <= temp / d; d += 2) {
                if (temp % d == 0) { p = d; break; }
            }
            if (d > temp / d) p = temp;   /* temp itself is prime */
        }

        /* count occurrences of p and toggle parity each time */
        while (temp % p == 0) {
            temp /= p;
            even ^= 1;
        }
    }

    return even ? (seq_t)1 : (seq_t)0;
}
